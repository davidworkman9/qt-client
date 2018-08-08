/**
* Tests for QDomDocument
*/
var tmp = new QDomDocument()
    , properties = []
    , publicFunctions = [
        'createAttribute'
        , 'createAttributeNS'
        , 'createCDATASection'
        , 'createComment'
        , 'createDocumentFragment'
        , 'createElement'
        , 'createElementNS'
        , 'createEntityReference'
        , 'createProcessingInstruction'
        , 'createTextNode'
        , 'doctype'
        , 'documentElement'
        , 'elementById'
        , 'elementsByTagName'
        , 'implementation'
        , 'importNode'
        , 'setContent'
        , 'toByteArray'
    ];

assertIsConstructor(QDomDocument, 'QDomDocument');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomDocument', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
