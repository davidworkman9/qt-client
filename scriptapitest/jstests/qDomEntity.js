/**
* Tests for QDomEntity
*/
var tmp = new QDomEntity()
    , properties = []
    , publicFunctions = [
        'nodeType'
        , 'notationName'
        , 'publicId'
        , 'systemId'
    ];

assertIsConstructor(QDomEntity, 'QDomEntity');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomEntity', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
