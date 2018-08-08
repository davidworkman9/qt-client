/**
* Tests for QBuffer
*/
var tmp = new QBuffer()
    , properties = [] // none at this time
    , publicFunctions = [
        'buffer'
        , 'data'
        , 'setBuffer'
        , 'setData'
    ];

assertIsConstructor(QBuffer, 'QBuffer');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QBuffer', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
