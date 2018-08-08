/**
* Test the QBoxLayout object
*/
var tmp = new QBoxLayout()
    , properties = []
    , publicFunctions = [
        'addLayout'
    ];

assertIsConstructor(QBoxLayout, 'QBoxLayout');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QBoxLayout', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    console.log('func name: ', func);
    assertIsFunction(tmp[func], func);
});
