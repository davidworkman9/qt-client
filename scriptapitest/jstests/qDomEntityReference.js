/**
* Tests for QDomEntityReference
*/
var tmp = new QDomEntityReference()
    , properties = []
    , publicFunctions = [
        'nodeType'
    ];

assertIsConstructor(QDomEntityReference, 'QDomEntityReference');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomEntityReference', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
