/**
* Tests for QDomComment
*/
var tmp = new QDomComment()
    , properties = []
    , publicFunctions = [
        'nodeType'
    ];

assertIsConstructor(QDomComment, 'QDomComment');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomComment', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
