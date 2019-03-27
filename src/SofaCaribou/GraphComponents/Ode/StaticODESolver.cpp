#include "StaticODESolver.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/AdvancedTimer.h>
#include <sofa/simulation/MechanicalOperations.h>
#include <sofa/simulation/VectorOperations.h>
#include <sofa/simulation/PropagateEventVisitor.h>

namespace SofaCaribou {
namespace GraphComponents {
namespace ode {

using sofa::core::VecId;
using namespace sofa::defaulttype;
using namespace sofa::core::behavior;

StaticODESolver::StaticODESolver()
    : d_newton_iterations(initData(&d_newton_iterations,
            (unsigned) 1,
            "newton_iterations",
            "Number of newton iterations between each load increments (normally, one load increment per simulation time-step."))
    , d_correction_tolerance_threshold(initData(&d_correction_tolerance_threshold,
            (double) 1e-5,
            "correction_tolerance_threshold",
            "Convergence criterion: The newton iterations will stop when the norm of correction |du| reach this threshold."))
    , d_residual_tolerance_threshold( initData(&d_residual_tolerance_threshold,
            (double) 1e-5,
            "residual_tolerance_threshold",
            "Convergence criterion: The newton iterations will stop when the norm of the residual |f - K(u)| reach this threshold. "
            "Use a negative value to disable this criterion."))
    , d_shoud_diverge_when_residual_is_growing( initData(&d_shoud_diverge_when_residual_is_growing,
            false,
            "shoud_diverge_when_residual_is_growing",
            "Divergence criterion: The newton iterations will stop when the residual is greater than the one from the previous iteration."))
    , d_converged(initData(&d_converged, false, "converged", "Whether or not the last call to solve converged", true /*is_displayed_in_gui*/, true /*is_read_only*/))

{}

void StaticODESolver::solve(const sofa::core::ExecParams* params, double /*dt*/, sofa::core::MultiVecCoordId xResult, sofa::core::MultiVecDerivId /*vResult*/) {
    sofa::simulation::common::VectorOperations vop( params, this->getContext() );
    sofa::simulation::common::MechanicalOperations mop( params, this->getContext() );
    sofa::simulation::common::VisitorExecuteFunc executeVisitor(*this->getContext());

    MultiVecCoord x_start(&vop, sofa::core::VecCoordId::position() );
    MultiVecCoord x(&vop, xResult /*core::VecCoordId::position()*/ );
    MultiVecDeriv force( &vop, sofa::core::VecDerivId::force() );
    dx.realloc( &vop, true );

    // MO vector dx is not allocated by default, it will seg fault if the CG is used (dx is taken by default) with an IdentityMapping
    MultiVecDeriv tempdx(&vop, sofa::core::VecDerivId::dx() ); tempdx.realloc( &vop, true, true );

    // Set implicit param to true to trigger nonlinear stiffness matrix recomputation
    mop->setImplicit(true);

    msg_info() << "======= Starting static ODE solver in time step " << this->getTime();
    msg_info() << "(doing a maximum of " << d_newton_iterations.getValue() << " newton iterations)";

    unsigned n_it=0;
    double dx_norm = -1.0, f_norm;

    sofa::helper::AdvancedTimer::stepBegin("StaticODESolver::Solve");

    // compute addForce, in mapped: addForce + applyJT (vec)
    // Initial computation
    force.clear();
    mop.computeForce(force);
    mop.projectResponse(force);
    f_norm = sqrt(force.dot(force));

    if (d_residual_tolerance_threshold.getValue() > 0 && f_norm <= d_residual_tolerance_threshold.getValue()) {
        msg_info() << "The ODE has already reached an equilibrium state";
    } else {

        while (n_it < d_newton_iterations.getValue()) {
            std::string stepname = "step_" + std::to_string(n_it);
            sofa::helper::AdvancedTimer::stepBegin(stepname.c_str());


            // assemble matrix, CG: does nothing
            // LDL non-mapped: addKToMatrix added to system matrix
            // LDL mapped: addKToMatrix not added to the system matrix, needs mapped FF (TODO)
            sofa::core::behavior::MultiMatrix<sofa::simulation::common::MechanicalOperations> matrix(&mop);
            matrix = MechanicalMatrix::K * -1.0;

            // for CG: calls iteratively addDForce, mapped:  [applyJ, addDForce, applyJt(vec)]+
            // for LDL: solves the system, everything's already assembled
            matrix.solve(dx, force);

            x.eq(x_start, dx, 1);
            mop.solveConstraint(x, sofa::core::ConstraintParams::POS);

            //propagate positions to mapped nodes: taken from AnimateVisitor::processNodeTopDown executed by the animation loop
            // calls apply, applyJ
            sofa::core::MechanicalParams mp;
            sofa::simulation::MechanicalPropagateOnlyPositionAndVelocityVisitor(&mp).execute(
                this->getContext()); // propagate the changes to mappings below

            // compute addForce, in mapped: addForce + applyJT (vec)
            force.clear();
            mop.computeForce(force);
            mop.projectResponse(force);
            double f_cur_norm = sqrt(force.dot(force));

            dx_norm = sqrt(dx.dot(dx));


            msg_info() << "Newton iteration #" << n_it << ": |f - K(x0 + dx)| = " << f_cur_norm << " |dx| = " << dx_norm;
            sofa::helper::AdvancedTimer::valSet("residual", f_cur_norm);
            sofa::helper::AdvancedTimer::valSet("correction", dx_norm);
            sofa::helper::AdvancedTimer::stepEnd(stepname.c_str());

            if (d_shoud_diverge_when_residual_is_growing.getValue() && f_cur_norm > f_norm && n_it>1) {
                d_converged.setValue(false);
                msg_info() << "[DIVERGED] residual's norm increased";
                break;
            }

            if (dx_norm <= this->d_correction_tolerance_threshold.getValue()) {
                d_converged.setValue(true);
                msg_info() << "[CONVERGED] The correction's norm |dx| is smaller than the threshold of "
                           << d_correction_tolerance_threshold;
                break;
            }

            if (d_residual_tolerance_threshold.getValue() > 0 && f_cur_norm <= d_residual_tolerance_threshold.getValue()) {
                d_converged.setValue(true);
                msg_info() << "[CONVERGED] The residual's norm |f - K(x0 + dx)| is smaller than the threshold of "
                           << d_residual_tolerance_threshold;
                break;
            }

            f_norm = f_cur_norm;
            n_it++;

        } // End while (n_it < d_newton_iterations.getValue())
    }

    if (n_it >= d_newton_iterations.getValue()) {
        n_it--;
        d_converged.setValue(false);
        msg_info() << "[DIVERGED] The number of Newton iterations reached the threshold of " << d_newton_iterations << " iterations";
    }

    sofa::helper::AdvancedTimer::valSet("nb_iterations", n_it+1);
    sofa::helper::AdvancedTimer::valSet("residual", f_norm);
    sofa::helper::AdvancedTimer::valSet("correction", dx_norm);
    sofa::helper::AdvancedTimer::stepEnd("StaticODESolver::Solve");
}


SOFA_DECL_CLASS(StaticODESolver)

int StaticODESolverClass = sofa::core::RegisterObject("Static ODE Solver")
    .add< StaticODESolver >()
;

} // namespace ode
} // namespace GraphComponents
} // namespace SofaCaribou