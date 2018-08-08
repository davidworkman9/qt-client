/**
* Tests for QDomElement
*/
var tmp = new QDomElement()
    , properties = []
    , publicFunctions = [
        'attribute'
        , 'attributeNS'
        , 'attributeNodeNS'
        , 'attributes'
        , 'elementsByTagName'
        , 'elementsByTagNameNS'
        , 'hasAttribute'
        , 'hasAttributeNS'
        , 'nodeType'
        , 'removeAttribute'
        , 'removeAttributeNS'
        , 'removeAttributeNode'
        , 'setAttribute'
        , 'setAttributeNS'
        , 'setAttributeNode'
        , 'setTagName'
        , 'tagName'
        , 'text'
    ];

assertIsConstructor(QDomElement, 'QDomElement');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomElement', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
