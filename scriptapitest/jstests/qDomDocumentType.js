/**
* Tests for QDomDocumentType
*/
var tmp = new QDomDocumentType()
    , properties = []
    , publicFunctions = [
        'entities'
        , 'internalSubset'
        , 'name'
        , 'nodeType'
        , 'notations'
        , 'publicId'
        , 'systemId'
    ];

assertIsConstructor(QDomDocumentType, 'QDomDocumentType');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomDocumentType', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
