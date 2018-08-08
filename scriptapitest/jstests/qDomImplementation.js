/**
* Tests for QDomImplementation
*/
var tmp = new QDomImplementation()
    , properties = []
    , publicFunctions = [
        'createDocument'
        , 'createDocumentType'
        , 'hasFeature'
        , 'isNull'
    ];

assertIsConstructor(QDomImplementation, 'QDomImplementation');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomImplementation', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
