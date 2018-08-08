/**
* Tests for QDomDocumentFragment
*/
var tmp = new QDomDocumentFragment()
    , properties = []
    , publicFunctions = [
        'nodeType'
    ];

assertIsConstructor(QDomDocumentFragment, 'QDomDocumentFragment');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomComQDomDocumentFragmentment', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
