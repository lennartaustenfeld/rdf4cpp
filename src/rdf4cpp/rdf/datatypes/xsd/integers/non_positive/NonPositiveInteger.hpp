#ifndef RDF4CPP_XSD_NONPOSITIVEINTEGER_HPP
#define RDF4CPP_XSD_NONPOSITIVEINTEGER_HPP

#include <rdf4cpp/rdf/datatypes/registry/DatatypeMapping.hpp>
#include <rdf4cpp/rdf/datatypes/registry/LiteralDatatypeImpl.hpp>
#include <rdf4cpp/rdf/datatypes/xsd/integers/signed/Integer.hpp>
#include <rdf4cpp/rdf/datatypes/registry/FixedIdMappings.hpp>

#include <boost/multiprecision/cpp_int.hpp>

namespace rdf4cpp::rdf::datatypes::registry {

template<>
struct DatatypeMapping<xsd_non_positive_integer> {
    using cpp_datatype = boost::multiprecision::cpp_int;
};

template<>
struct DatatypeSupertypeMapping<xsd_non_positive_integer> {
    using supertype = xsd::Integer;
};

template<>
struct DatatypeNumericStubMapping<xsd_non_positive_integer> {
    using numeric_impl_type = xsd::Integer;
};

template<>
capabilities::Default<xsd_non_positive_integer>::cpp_type capabilities::Default<xsd_non_positive_integer>::from_string(std::string_view s);

template<>
bool capabilities::Logical<xsd_non_positive_integer>::effective_boolean_value(cpp_type const &value) noexcept;

template<>
std::partial_ordering capabilities::Comparable<xsd_non_positive_integer>::compare(cpp_type const &lhs, cpp_type const &rhs) noexcept;

template<>
nonstd::expected<capabilities::Default<xsd_non_positive_integer>::cpp_type, DynamicError> capabilities::Subtype<xsd_non_positive_integer>::from_supertype(super_cpp_type const &value) noexcept;

extern template struct LiteralDatatypeImpl<xsd_non_positive_integer,
                                           capabilities::Logical,
                                           capabilities::NumericStub,
                                           capabilities::Subtype,
                                           capabilities::Comparable,
                                           capabilities::FixedId>;

}  // namespace rdf4cpp::rdf::datatypes::registry


namespace rdf4cpp::rdf::datatypes::xsd {

struct NonPositiveInteger : registry::LiteralDatatypeImpl<registry::xsd_non_positive_integer,
                                                          registry::capabilities::Logical,
                                                          registry::capabilities::NumericStub,
                                                          registry::capabilities::Subtype,
                                                          registry::capabilities::Comparable,
                                                          registry::capabilities::FixedId> {};

} // namespace rdf4cpp::rdf::datatypes::xsd


namespace rdf4cpp::rdf::datatypes::registry::instantiation_detail {

[[maybe_unused]] inline xsd::NonPositiveInteger const xsd_non_positive_integer_instance;

} // namespace rdf4cpp::rdf::datatypes::registry::instantiation_detail

#endif  //RDF4CPP_XSD_NONPOSITIVEINTEGER_HPP