#ifndef RDF4CPP_NODETYPESTORAGE_HPP
#define RDF4CPP_NODETYPESTORAGE_HPP

#include <rdf4cpp/rdf/storage/util/tsl/sparse_map.h>

#include <rdf4cpp/rdf/storage/node/identifier/NodeID.hpp>

#include <memory>
#include <shared_mutex>


namespace rdf4cpp::rdf::storage::node::reference_node_storage {



/**
 * Storage for one of the Node Backend types. Includes a shared mutex to synchronize access and bidirectional mappings between the Backend type and identifier::NodeID.
 * @tparam BackendType_t one of BNodeBackend, IRIBackend, FallbackLiteralBackend, SpecializedLiteralBackend and VariableBackend.
 */
template<class BackendType_t>
struct NodeTypeStorage {
    using Backend = BackendType_t;
    using BackendView = typename Backend::View;
private:
    struct DefaultBackendTypeEqual {
        using is_transparent = void;

        bool operator()(Backend const *lhs, Backend const *rhs) const noexcept {
            return lhs == rhs;
        }
        bool operator()(BackendView const &lhs, Backend const *rhs) const noexcept {
            return lhs == BackendView(*rhs);
        }

        bool operator()(Backend const *lhs, BackendView const &rhs) const noexcept {
            return BackendView(*lhs) == rhs;
        }
    };

    template<typename T>
    struct SelectBackendTypeEqual {
        using type = DefaultBackendTypeEqual;
    };

    template<typename T> requires requires { typename T::Equal; }
    struct SelectBackendTypeEqual<T> {
        using type = typename T::Equal;
    };

    struct DefaultBackendTypeHash {
        [[nodiscard]] size_t operator()(Backend const *x) const noexcept {
            return x->hash();
        }
        [[nodiscard]] size_t operator()(BackendView const &x) const noexcept {
            return x.hash();
        }
    };

    template<typename T>
    struct SelectBackendTypeHash {
        using type = DefaultBackendTypeHash;
    };

    template<typename T> requires requires { typename T::Hash; }
    struct SelectBackendTypeHash<T> {
        using type = typename T::Hash;
    };

public:
    using BackendTypeEqual = typename SelectBackendTypeEqual<BackendType_t>::type;
    using BackendTypeHash = typename SelectBackendTypeHash<BackendType_t>::type;

    struct NodeIDHash {
        [[nodiscard]] size_t operator()(identifier::NodeID const &x) const noexcept {
            return x.value();
        }
    };

    std::shared_mutex mutable mutex;
    util::tsl::sparse_map<identifier::NodeID, std::unique_ptr<Backend>, NodeIDHash> id2data;
    util::tsl::sparse_map<Backend *, identifier::NodeID, BackendTypeHash, BackendTypeEqual> data2id;
};
}  // namespace rdf4cpp::rdf::storage::node::reference_node_storage

#endif  //RDF4CPP_NODETYPESTORAGE_HPP
