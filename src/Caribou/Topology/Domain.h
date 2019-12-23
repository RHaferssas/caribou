#ifndef CARIBOU_TOPOLOGY_DOMAIN_H
#define CARIBOU_TOPOLOGY_DOMAIN_H

#include <Caribou/config.h>
#include <Caribou/macros.h>
#include <Caribou/Constants.h>
#include <Caribou/Topology/BaseDomain.h>
#include <Caribou/Geometry2/Traits/Element.h>

#include <vector>
#include <array>

namespace caribou::topology {
    namespace internal {
        template <typename Element, class Enable = void>
        class ElementStorage;
    }

    /*!
     * A domain implements the BaseDomain interface with a given element type.
     *
     * @tparam Element See caribou::geometry::is_an_element
     */
    template <typename Mesh, typename Element>
    class Domain : public BaseDomain, private internal::ElementStorage<Element> {
        static_assert(caribou::geometry::is_an_element_v<Element>, "Trying to specialize a domain with a non element type.");

        friend Mesh;
    public:
        /*!
         * If the Element type as a static number of nodes at compile time, than Indices is
         *  std::vector<std::array<unsigned int, Element::NumberOfNodesAtCompileTime>>
         * else, if the Element type as a dynamic number of nodes (known at runtime), than Indices is
         *  std::vector<std::vector<unsigned int>>
         */
        using Indices = typename internal::ElementStorage<Element>::Indices;

        explicit Domain(const Mesh * mesh) : p_mesh(mesh) {
            if (!mesh) {
                throw std::runtime_error("Trying to instantiate a domain without mesh.");
            }
        }

        /**
         * Get the mesh associated with this domain.
         */
        [[nodiscard]] inline auto mesh() const -> const Mesh & {
            return *p_mesh;
        }

        /*!
         * \copydoc caribou::topology::BaseDomain::canonical_dimension
         */
        [[nodiscard]] auto canonical_dimension() const -> UNSIGNED_INTEGER_TYPE final {
            return Element::CanonicalDimension;
        }

        /*!
         * \copydoc caribou::topology::BaseDomain::number_of_elements
         */
        [[nodiscard]] auto number_of_elements() const -> UNSIGNED_INTEGER_TYPE final {
            return this->p_elements.size();
        };

        /*!
         * Add an element to the domain.
         * @param indices The indices of its nodes in the mesh's position vector.
         */
        inline void add(const Indices & element) {
            this->p_elements.push_back(element);
        }

        /*!
         * Get the indices of an element in the domain.
         * @param index The index of the element.
         * @return The indices of its nodes in the mesh's position vector.
         */
        inline auto element_indices(const UNSIGNED_INTEGER_TYPE & index) const -> const Indices & {
            caribou_assert(index < this->p_elements.size() and "Trying to get an element that does not exists.");

            return this->p_elements[index];
        }

        /*!
         * Get an element from its index.
         * @param element_index The index of the element.
         * @return An instance of the element.
         */
        inline auto element(const UNSIGNED_INTEGER_TYPE & index) const -> Element {
            // Get the indices of the element nodes
            const auto indices = element_indices(index);

            // Get the mesh
            const auto & mesh = this->mesh();

            // Fill-in the position vector of element nodes
            if constexpr (Element::NumberOfNodesAtCompileTime == caribou::Dynamic) {
                std::vector<typename Mesh::WorldCoordinate> positions (indices.size());
                mesh.get_positions(indices, positions);
                return Element(positions.data(), positions.size());
            } else {
                std::array<typename Mesh::WorldCoordinate, Element::NumberOfNodesAtCompileTime> positions;
                mesh.get_positions(indices, positions);
                return Element(positions.data()->data());
            }
        }

    private:
        const Mesh * p_mesh;

    };

    namespace internal {
        template<typename Element, class Enable>
        class ElementStorage {
        protected:
            using Indices = std::vector<UNSIGNED_INTEGER_TYPE>;
            std::vector<Indices> p_elements;
        };

        template<typename Element>
        class ElementStorage<Element, CLASS_REQUIRES(Element::NumberOfNodesAtCompileTime != caribou::Dynamic)> {
        protected:
            using Indices = std::array<UNSIGNED_INTEGER_TYPE, Element::NumberOfNodesAtCompileTime>;
            std::vector<Indices> p_elements;
        };
    }
}

#endif //CARIBOU_TOPOLOGY_DOMAIN_H
