#ifndef RDF4CPP_RDFFILEPARSER_HPP
#define RDF4CPP_RDFFILEPARSER_HPP

#include <rdf4cpp/rdf/parser/IStreamQuadIterator.hpp>
#include <fstream>

namespace rdf4cpp::rdf::parser {
/**
     * Similar to rdf4cpp::rdf::parser::IStreamQuadIterator
     * Parses the file by the given path and tries to extract Quads given in TURTLE format.
     *
     * @note the iterator _starts on_ the first Quad
     * @note An exhausted iterator becomes the end-of-stream iterator.
     * @warning The value pointed to by an end-of-stream iterator is undefined
     *
     * @example
     * @code
     * for (const auto& e : rdf4cpp::rdf::parser::RDFFileParser{"somewhere.ttl"}) {
     *  if (e.has_value())
     *     std::cout << e.value();
     *  else
     *     std::cout << e.error();
     * }
     * @endcode
     */
class RDFFileParser {
    std::string file_path_;
    ParsingFlags flags_;
    storage::node::NodeStorage node_storage_;

public:
    explicit RDFFileParser(const std::string &file_path, ParsingFlags flags = ParsingFlags::none(),
                           storage::node::NodeStorage node_storage = storage::node::NodeStorage::default_instance());
    explicit RDFFileParser(std::string &&file_path, ParsingFlags flags = ParsingFlags::none(),
                           storage::node::NodeStorage node_storage = storage::node::NodeStorage::default_instance());

    class Iterator {
        friend class RDFFileParser;
        friend bool operator==(const RDFFileParser::Iterator &iter, std::default_sentinel_t) noexcept;
        std::unique_ptr<std::ifstream> stream_;
        std::unique_ptr<IStreamQuadIterator> iter_;

        Iterator();
        Iterator(std::ifstream &&stream, ParsingFlags flags, const storage::node::NodeStorage &node_storage);

    public:
        using value_type = IStreamQuadIterator::value_type;
        using reference = IStreamQuadIterator::reference;
        using pointer = IStreamQuadIterator::pointer;
        using difference_type = IStreamQuadIterator::difference_type;
        using iterator_category = IStreamQuadIterator::iterator_category;
        using istream_type = IStreamQuadIterator::istream_type;

        reference operator*() const noexcept;
        pointer operator->() const noexcept;
        Iterator &operator++();
        [[nodiscard]] bool operator==(const Iterator &other) const noexcept;
        // != gets generated by compiler
    };

    [[nodiscard]] Iterator begin() const;
    std::default_sentinel_t end() const noexcept;
};
[[nodiscard]] bool operator==(const RDFFileParser::Iterator &iter, std::default_sentinel_t s) noexcept;
[[nodiscard]] bool operator==(std::default_sentinel_t s, const RDFFileParser::Iterator &iter) noexcept;
}  // namespace rdf4cpp::rdf::parser

#endif  //RDF4CPP_RDFFILEPARSER_HPP