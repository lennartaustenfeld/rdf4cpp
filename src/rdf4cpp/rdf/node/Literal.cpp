#include "Literal.h"

#include <rdf4cpp/rdf/graph/node_manager/LiteralBackend.h>
#include <rdf4cpp/rdf/node/IRIResource.h>

namespace rdf4cpp::rdf::node {

Literal::Literal(void *ptr, const RDFNode::NodeID &id) : RDFNode(ptr, id) {}
Literal::Literal() : RDFNode() {}
Literal::Literal(const std::string &lexical_form, RDFNode::NodeManager &node_manager)
    : RDFNode(BackendNodeHandle{node_manager.get_string_literal(lexical_form).first, RDFNodeType::Literal}) {}
Literal::Literal(const std::string &lexical_form, const IRIResource &datatype, RDFNode::NodeManager &node_manager)
    : RDFNode(BackendNodeHandle{
              node_manager.get_typed_literal(
                                  lexical_form,
                                  node_manager.lookup_id(&datatype.handle_.iri_backend(), RDFNodeType::IRI))
                      .first,
              RDFNodeType::Literal}) {}
Literal::Literal(const std::string &lexical_form, const std::string &lang, RDFNode::NodeManager &node_manager)
    : RDFNode(BackendNodeHandle{node_manager.get_lang_literal(lexical_form, lang).first, RDFNodeType::Literal}) {}


IRIResource Literal::datatype(NodeManager &node_manager) const {
    NodeID datatype_id = handle_.literal_backend().datatype_id();
    return IRIResource(node_manager.lookup_iri(datatype_id), datatype_id);
}
const std::string &Literal::lexical_form() const {
    return handle_.literal_backend().lexical_form();
}

const std::string &Literal::language_tag() const {
    return handle_.literal_backend().language_tag();
}
std::string Literal::as_string(bool quoting, NodeManager &node_manager) const {
    const auto &literal = handle_.literal_backend();
    if (not quoting and literal.datatype_id() == NodeID::xsd_string_iri.first) {
        // TODO: support non datatype literals?
        return literal.lexical_form();
    } else {
        if (literal.datatype_id() == NodeID::xsd_string_iri.first) {
            return literal.quote_lexical();
        } else if (literal.datatype_id() == NodeID::xsd_langstring_iri.first) {
            return literal.quote_lexical() + "@" + literal.language_tag();
        } else {
            return literal.quote_lexical() + "^^" + node_manager.lookup_iri(literal.datatype_id())->as_string(true);
        }
    }
}
bool Literal::is_blank_node() const { return false; }
bool Literal::is_literal() const { return true; }
bool Literal::is_variable() const { return false; }
bool Literal::is_bnode() const { return false; }
bool Literal::is_iri() const { return false; }
RDFNode::RDFNodeType Literal::type() const { return RDFNodeType::Literal; }
Literal::Literal(RDFNode::BackendNodeHandle handle) : RDFNode(handle) {}

}  // namespace rdf4cpp::rdf::node