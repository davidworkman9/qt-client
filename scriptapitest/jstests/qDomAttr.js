/**
* Tests for QDomAttr
*/
var tmp = new QDomAttr()
    , properties = []
    , publicFunctions = [
        'name'
        , 'nodeType'
        , 'ownerElement'
        , 'setValue'
        , 'specified'
        , 'value'
    ];

assertIsConstructor(QDomAttr, 'QDomAttr');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomAttr', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
