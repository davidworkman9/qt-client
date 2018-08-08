/**
* Tests for the QButtonGroup
*/
var tmp = QButtonGroup()
    , properties = [
        'exclusive'
        , 'button'
        , 'checkedButton'
        , 'checkedId'
        , 'id'
        , 'setId'
    ]
    , publicFunctions = [
        'addButton'
        , 'buttons'
        , 'removeButton'
        , 'setExclusive'
    ];

assertIsConstructor(QButtonGroup, 'QButtonGroup');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QButtonGroup', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
