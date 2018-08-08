/**
* Tests for QDomCharacterData
*/
var tmp = new QDomCharacterData()
    , properties = []
    , publicFunctions = [
        'appendData'
        , 'data'
        , 'deleteData'
        , 'insertData'
        , 'length'
        , 'nodeType'
        , 'replaceData'
        , 'setData'
        , 'substringData'
    ];

assertIsConstructor(QDomCharacterData, 'QDomCharacterData');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomCharacterData', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
