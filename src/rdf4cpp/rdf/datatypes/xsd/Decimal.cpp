#include <rdf4cpp/rdf/datatypes/xsd/Decimal.hpp>

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <regex>

namespace rdf4cpp::rdf::datatypes::registry {

template<>
capabilities::Default<xsd_decimal>::cpp_type capabilities::Default<xsd_decimal>::from_string(std::string_view s) {
    // https://www.w3.org/TR/xmlschema11-2/#decimal
    static std::regex const decimal_regex{R"#((\+|-)?([0-9]+(\.[0-9]*)?|\.[0-9]+))#", std::regex_constants::optimize};

    if (!std::regex_match(s.begin(), s.end(), decimal_regex)) {
        throw std::runtime_error{"XSD Parsing Error"};
    }

    if (s.starts_with('+')) {
        s.remove_prefix(1);
    }

    return cpp_type{s};
}

template<>
std::string capabilities::Default<xsd_decimal>::to_canonical_string(cpp_type const &value) noexcept {
    auto s = value.str(std::numeric_limits<cpp_type>::digits10, std::ios_base::fixed | std::ios_base::showpoint);
    auto const non_zero_pos = s.find_last_not_of('0');

    // cannot be npos because, showpoint is set and '.' != '0'
    assert(non_zero_pos != std::string::npos);

    // dot was found char implies there is a char after the dot (because precision == digits10 > 0) and char after dot must be zero
    assert(s[non_zero_pos] != '.' || (s.size() >= non_zero_pos + 2 && s[non_zero_pos + 1] == '0'));

    // +1 for pos -> size conversion
    // maybe +1 to include one zero after dot
    s.resize(non_zero_pos + 1 + static_cast<std::string::size_type>(s[non_zero_pos] == '.'));

    return s;
}

template<>
std::string capabilities::Default<xsd_decimal>::to_simplified_string(cpp_type const &value) noexcept {
    return value.str();
}

template<>
nonstd::expected<capabilities::Numeric<xsd_decimal>::add_result_cpp_type, DynamicError> capabilities::Numeric<xsd_decimal>::add(cpp_type const &lhs, cpp_type const &rhs) noexcept {
    // https://www.w3.org/TR/xpath-functions/#op.numeric
    // decimal needs overflow protection

    auto res = lhs + rhs;
    if (isinf(res)) [[unlikely]] {
        return nonstd::make_unexpected(DynamicError::OverOrUnderFlow);
    }

    return res;
}

template<>
nonstd::expected<capabilities::Numeric<xsd_decimal>::sub_result_cpp_type, DynamicError> capabilities::Numeric<xsd_decimal>::sub(cpp_type const &lhs, cpp_type const &rhs) noexcept {
    // https://www.w3.org/TR/xpath-functions/#op.numeric
    // decimal needs overflow protection

    auto res = lhs - rhs;
    if (isinf(res)) [[unlikely]] {
        return nonstd::make_unexpected(DynamicError::OverOrUnderFlow);
    }

    return res;
}

template<>
nonstd::expected<capabilities::Numeric<xsd_decimal>::div_result_cpp_type, DynamicError> capabilities::Numeric<xsd_decimal>::div(cpp_type const &lhs, cpp_type const &rhs) noexcept {
    // https://www.w3.org/TR/xpath-functions/#func-numeric-divide
    // decimal needs error (and cpp_type is not integral) && overflow protection

    if (rhs == 0) {
        return nonstd::make_unexpected(DynamicError::DivideByZero);
    }

    auto res = lhs / rhs;
    if (isinf(res)) [[unlikely]] {
        return nonstd::make_unexpected(DynamicError::OverOrUnderFlow);
    }

    return res;
}

template<>
nonstd::expected<capabilities::Numeric<xsd_decimal>::mul_result_cpp_type, DynamicError> capabilities::Numeric<xsd_decimal>::mul(cpp_type const &lhs, cpp_type const &rhs) noexcept {
    // https://www.w3.org/TR/xpath-functions/#op.numeric
    // decimal needs overflow protection

    auto res = lhs * rhs;
    if (isinf(res)) [[unlikely]] {
        return nonstd::make_unexpected(DynamicError::OverOrUnderFlow);
    }

    return res;
}

template<>
nonstd::expected<capabilities::Numeric<xsd_decimal>::abs_result_cpp_type, DynamicError> capabilities::Numeric<xsd_decimal>::abs(cpp_type const &operand) noexcept {
    return boost::multiprecision::abs(operand);
}

template<>
nonstd::expected<capabilities::Numeric<xsd_decimal>::round_result_cpp_type, DynamicError> capabilities::Numeric<xsd_decimal>::round(cpp_type const &operand) noexcept {
    return boost::multiprecision::round(operand);
}

template<>
nonstd::expected<capabilities::Numeric<xsd_decimal>::floor_result_cpp_type, DynamicError> capabilities::Numeric<xsd_decimal>::floor(cpp_type const &operand) noexcept {
    return boost::multiprecision::floor(operand);
}

template<>
nonstd::expected<capabilities::Numeric<xsd_decimal>::ceil_result_cpp_type, DynamicError> capabilities::Numeric<xsd_decimal>::ceil(cpp_type const &operand) noexcept {
    return boost::multiprecision::ceil(operand);
}

template<>
bool capabilities::Logical<xsd_decimal>::effective_boolean_value(cpp_type const &value) noexcept {
    return !isnan(value) && value != 0.0;
}

template<>
std::partial_ordering capabilities::Comparable<xsd_decimal>::compare(cpp_type const &lhs, cpp_type const &rhs) noexcept {
    if (lhs < rhs) {
        return std::partial_ordering::less;
    } else if (rhs < lhs) {
        return std::partial_ordering::greater;
    } else {
        return std::partial_ordering::equivalent;
    }
}

template struct LiteralDatatypeImpl<xsd_decimal,
                                    capabilities::Logical,
                                    capabilities::Numeric,
                                    capabilities::Comparable,
                                    capabilities::Promotable,
                                    capabilities::FixedId>;

}  // namespace rdf4cpp::rdf::datatypes::registry
