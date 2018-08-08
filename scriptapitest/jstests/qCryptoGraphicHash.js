/**
* Tests for QCryptographicHash
*/
var tmp = new QCryptographicHash(QCryptographicHash.Md4)
    , properties = []
    , publicFunctions = [
        'addData'
        , 'reset'
        , 'result'
    ];

assertIsConstructor(QCryptographicHash, 'QCryptographicHash', [QCryptographicHash.Md4]);

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QCoreApplication', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
