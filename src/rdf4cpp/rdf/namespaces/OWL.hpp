#ifndef RDF4CPP_OWL_HPP
#define RDF4CPP_OWL_HPP

#include <rdf4cpp/rdf/ClosedNamespace.hpp>
#include <rdf4cpp/rdf/namespaces/util/NamespaceInstance.hpp>

namespace rdf4cpp::rdf::namespaces {

/**
 * A ClosedNamespace for http://www.w3.org/2002/07/owl#
 */
class OWL : public ClosedNamespace, public util::NamespaceInstance<OWL> {
public:
    static constexpr std::string_view NAMESPACE = "http://www.w3.org/2002/07/owl#";
    static constexpr std::array<std::string_view, 83> SUFFIXES = {"AllDifferent", "AllDisjointClasses", "AllDisjointProperties", "allValuesFrom", "annotatedProperty", "annotatedSource", "annotatedTarget", "Annotation", "AnnotationProperty", "assertionProperty", "AsymmetricProperty", "Axiom", "backwardCompatibleWith", "bottomDataProperty", "bottomObjectProperty", "cardinality", "Class", "complementOf", "DataRange", "datatypeComplementOf", "DatatypeProperty", "dateTime", "deprecated", "DeprecatedClass", "DeprecatedProperty", "differentFrom", "disjointUnionOf", "disjointWith", "distinctMembers", "equivalentClass", "equivalentProperty", "FunctionalProperty", "hasKey", "hasSelf", "hasValue", "imports", "incompatibleWith", "intersectionOf", "InverseFunctionalProperty", "inverseOf", "IrreflexiveProperty", "maxCardinality", "maxQualifiedCardinality", "members", "minCardinality", "minQualifiedCardinality", "NamedIndividual", "NegativePropertyAssertion", "Nothing", "object", "ObjectProperty", "onClass", "onDataRange", "onDatatype", "oneOf", "onProperties", "onProperty", "Ontology", "OntologyProperty", "predicate", "priorVersion", "propertyChainAxiom", "propertyDisjointWith", "qualifiedCardinality", "rational", "real", "ReflexiveProperty", "Restriction", "sameAs", "someValuesFrom", "sourceIndividual", "subject", "SymmetricProperty", "targetIndividual", "targetValue", "Thing", "topDataProperty", "topObjectProperty", "TransitiveProperty", "unionOf", "versionInfo", "versionIRI", "withRestrictions"};

    explicit OWL(NodeStorage &node_storage = NodeStorage::default_instance());
};

}  // namespace rdf4cpp::rdf::namespaces

#endif  //RDF4CPP_OWL_HPP
