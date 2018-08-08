/**
* Tests for QDialogButtonBox
*/
var tmp = new QDialogButtonBox()
    , properties = [
        'centerButtons'
        , 'orientation'
        , 'standardButtons'
    ]
    , publicFunctions = [
        'addButton'
        , 'button'
        , 'buttonRole'
        , 'buttons'
        //, 'centerButtons'
        , 'clear'
        //, 'orientation'
        , 'removeButton'
        , 'setCenterButtons'
        , 'setOrientation'
        , 'setStandardButtons'
        , 'standardButton'
    ];

assertIsConstructor(QDialogButtonBox, 'QDialogButtonBox');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDialogButtonBox', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
