#ifndef CARIBOU_TOPOLOGY_MESH_H
#define CARIBOU_TOPOLOGY_MESH_H

#include <Caribou/config.h>
#include <Caribou/Topology/BaseMesh.h>
#include <Caribou/Topology/Domain.h>

#include <array>
#include <memory>
#include <unordered_map>

namespace caribou::topology {

    template <typename Derived>
    class Mesh : public BaseMesh {
    public:

        template<typename Element>
        using Domain = Domain<Derived, Element>;

        /*!
         * \copydoc caribou::topology::BaseMesh::dimension
         */
        [[nodiscard]] inline auto dimension() const -> UNSIGNED_INTEGER_TYPE final {
            return Derived::Dimension;
        };

        /*!
         * \copydoc caribou::topology::BaseMesh::number_of_domains
         */
        [[nodiscard]]
        inline auto number_of_domains() const -> UNSIGNED_INTEGER_TYPE final {
            return p_domains.size();
        }

        /*!
         * Adds a domain with the Element type supplied as a template parameter.
         * @tparam Element The type of Element
         * @param name
         * @return
         */
        template<typename Element>
        inline auto add_domain(const std::string & name) -> Domain<Element> * {
            if (p_domains.find(name) != p_domains.end()) {
                throw std::domain_error(std::string("A domain with the name ") + name + " already exists.");
            }
            auto domain_ptr = new Domain<Element>(static_cast<const Derived *>(this));
            auto res = p_domains.insert({name, std::unique_ptr<BaseDomain>(static_cast<BaseDomain *>(domain_ptr))});
            if (res.second) {
                return static_cast<Domain<Element>*> (res.first->second.get());
            } else {
                return static_cast<Domain<Element>*> (nullptr);
            }
        }

        /*!
        * Get the position coordinates of a node from its index.
        */
        [[nodiscard]] inline auto position(UNSIGNED_INTEGER_TYPE index) const {
            return self()._position(index);
        }

        /*!
         * Fills in a position vector from the given indices vector.
         * @tparam IndicesRange A const iterable range-based object of int (must have the cbegin() and cend() members)
         * @tparam PositionsRange  An iterable range-based object of WorldCoordinate (must have the begin() and end() members)
         * @param indices [IN] The indices from which the positions vector will be filled
         * @param positions [OUT] The position coordinates of every nodes queried.
         *
         * Example:
         * \code{.cpp}
         * std::array<unsigned int, 4> indices = {{0, 4, 12, 42}};
         * std::array<UnstructuredMesh<3>::WorldCoordinate, 4> positions;
         * mesh.get_positions(indices, positions);
         *
         * std::cout << "Position of node # << 12 << " is " << positions[2] << std::endl;
         * \endcode
         *
         */
        template <typename IndicesRange, typename PositionsRange>
        inline void get_positions(IndicesRange && indices, PositionsRange && positions) const {
            self()._positions(std::forward<IndicesRange>(indices), std::forward<PositionsRange>(positions));
        }

        /*!
         * Fills in a position vector from the given indices vector.
         * @tparam IntegerType Integer type of the indices passed
         * @tparam PositionsRange  An iterable range-based object of WorldCoordinate (must have the begin() and end() members)
         * @param indices [IN] The indices from which the positions vector will be filled
         * @param positions [OUT] The position coordinates of every nodes queried.
         *
         * Example:
         * \code{.cpp}
         * std::array<UnstructuredMesh<3>::WorldCoordinate, 4> positions;
         * mesh.get_positions({0,4,12,43}, positions);
         *
         * std::cout << "Position of node # << 12 << " is " << positions[2] << std::endl;
         * \endcode
         *
         */
        template <typename IntegerType, typename PositionsRange>
        inline void get_positions(std::initializer_list<IntegerType> && indices, PositionsRange && positions) const {
            return self()._positions(std::forward<std::initializer_list<IntegerType>>(indices), std::forward<PositionsRange>(positions));
        }

    private:
        auto self () const -> const Derived &
        {
            return static_cast<const Derived &>(*this);
        }


        std::unordered_map<std::string, std::unique_ptr<BaseDomain>> p_domains;
    };
}

#endif //CARIBOU_TOPOLOGY_MESH_H
