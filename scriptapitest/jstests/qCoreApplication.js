/**
* Tests for QCoreApplication
*/
var tmp = new QCoreApplication()
    , properties = []
    , publicFunctions = [];

assertIsConstructor(QCoreApplication, 'QCoreApplication');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QCoreApplication', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
