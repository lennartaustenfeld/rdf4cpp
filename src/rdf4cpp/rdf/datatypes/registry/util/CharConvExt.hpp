#ifndef RDF4CPP_REGISTRY_CHARCONVEXT_HPP
#define RDF4CPP_REGISTRY_CHARCONVEXT_HPP

#include <algorithm>
#include <cassert>
#include <charconv>
#include <cmath>
#include <concepts>
#include <stdexcept>

namespace rdf4cpp::rdf::datatypes::registry::util {

/**
 * Parses a valid string representation of a integral number
 *
 * @tparam I the resulting integral type
 * @param s the string to be parsed
 * @return the resulting value
 * @throws std::runtime_error if the string cannot be parsed
 */
template<std::integral I>
I from_chars(std::string_view s) {
    if (s.starts_with('+')) {
        // from_chars does not allow initial +
        s.remove_prefix(1);
    }

    I value;
    auto const res = std::from_chars(s.data(), s.data() + s.size(), value);

    if (res.ec != std::errc{} || res.ptr != s.data() + s.size()) {
        throw std::runtime_error{"xsd integer parsing error: " + std::make_error_code(res.ec).message()};
    } else {
        return value;
    }
}

/**
 * Serializes an integral type into its (SPARQL) canonical representation
 * see https://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#dt-integer
 *
 * @param value the value to be serialized
 */
template<std::integral I>
std::string to_chars(I const value) noexcept {
    // +1 because of definition of digits10 https://en.cppreference.com/w/cpp/types/numeric_limits/digits10
    // +1 for sign
    static constexpr size_t buf_sz = std::numeric_limits<I>::digits10 + 1 + static_cast<size_t>(std::is_signed_v<I>);

    std::array<char, buf_sz> buf;
    std::to_chars_result const res = std::to_chars(buf.data(), buf.data() + buf.size(), value);

    assert(res.ec == std::errc{});

    return std::string{buf.data(), res.ptr};
}

/**
 * Parses a valid string representation of a floating point number
 *
 * @tparam F the result floating point type
 * @param s the string to be parsed
 * @return the resulting value
 * @throws std::runtime_error if the string cannot be parsed
 */
template<std::floating_point F>
F from_chars(std::string_view s) {
    if (s.starts_with('+')) {
        // from_chars does not allow initial +
        s.remove_prefix(1);
    }

    F value;
    std::from_chars_result const res = std::from_chars(s.data(), s.data() + s.size(), value, std::chars_format::general);

    if (res.ec != std::errc{} || res.ptr != s.data() + s.size()) {
        // parsing did not reach end of string => it contains invalid characters
        throw std::runtime_error{"xsd floating point parsing error: " + std::make_error_code(res.ec).message()};
    }

    return value;
}

namespace detail  {
/**
 * equivalent to static_cast<size_t>(1 + std::log10(value))
 * only exists because the above is not a constexpr in clang
 */
template<typename T>
constexpr size_t log10ceil(T const value) noexcept {
    if (value < 10) {
        return 1;
    }
    return 1 + log10ceil(value / 10);
}
} // namespace detail

/**
 * Serializes a floating point number into its (SPARQL) canonical string representation
 * see https://www.w3.org/TR/2004/REC-xmlschema-2-20041028/#dt-float
 *
 * @param value the value to be serialized
 */
template<std::floating_point F>
std::string to_chars(F const value) noexcept {
    if (std::isnan(value)) {
        return "NaN";
    }

    if (std::isinf(value)) {
        if (value > 0) {
            return "INF";
        } else {
            return "-INF";
        }
    }

    // +1 for minus in mantissa
    // +1 for integral part
    // +1 for dot
    // +1 for E
    // +1 for minus in exponent
    // at least 2 for exponent because the (c++) standard says so (https://en.cppreference.com/w/cpp/utility/to_chars)
    static constexpr size_t buf_sz = 5 + std::numeric_limits<F>::max_digits10 + std::max(2ul, detail::log10ceil(std::numeric_limits<F>::max_exponent10));
    std::array<char, buf_sz> buf;

    std::to_chars_result res = [&]() {
        [[maybe_unused]] F integral;
        F const fraction = std::modf(value, &integral);

        if (fraction == 0) {
            // include one zero after dot
            return std::to_chars(buf.data(), buf.data() + buf.size(), value, std::chars_format::scientific, 1);
        } else {
            // automatic precision guarantees round-trip-safety (https://en.cppreference.com/w/cpp/utility/to_chars)
            return std::to_chars(buf.data(), buf.data() + buf.size(), value, std::chars_format::scientific);
        }
    }();
    assert(res.ec == std::errc{});

    auto *e_ptr = std::find(buf.data(), res.ptr, 'e');
    assert(e_ptr != res.ptr); // serializing in scientific notation, there must be an 'e'
    *e_ptr = 'E'; // convert 'e' to 'E' as required by the SPARQL standard

    auto const shift_amt = [&]() {
        auto *exp_start_ptr = e_ptr + 2;
        if (auto *non_zero_exp_ptr = std::find_if(exp_start_ptr, res.ptr, [](char const c) { return c != '0'; }); non_zero_exp_ptr != res.ptr) {
            // exponent has leading zeros
            // shift such that leading zeros get shifted out
            return non_zero_exp_ptr - exp_start_ptr;
        } else {
            // exponent is only zeros
            // shift such that one zero remains
            return res.ptr - exp_start_ptr - 1;
        }
    }();

    auto const shift_off = static_cast<std::string::size_type>(*(e_ptr + 1) == '+'); // needed to shift potential '+' out as well

    auto *new_end = std::shift_left(e_ptr + 1 + (1 - shift_off), res.ptr, shift_amt + shift_off); // shift out all leading zeros and plus sign from exponent
    return std::string{buf.data(), static_cast<std::string::size_type>(new_end - buf.data())};
}

} // namespace rdf4cpp::rdf::datatypes::registry::util

#endif  //RDF4CPP_REGISTRY_CHARCONVEXT_HPP