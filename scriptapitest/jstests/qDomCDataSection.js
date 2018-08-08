/**
* Tests for QDomCDATASection
*/
var tmp = new QDomCDATASection()
    , properties = []
    , publicFunctions = [
        'nodeType'
    ];

assertIsConstructor(QDomCDATASection, 'QDomCDATASection');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomCDATASection', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
