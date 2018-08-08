/**
* Tests for QDnsTextRecord
*/
var tmp = new QDnsTextRecord()
    , properties = []
    , publicFunctions = [
        'name'
        , 'swap'
        , 'timeToLive'
        , 'values'
    ];

assertIsConstructor(QDnsTextRecord, 'QDnsTextRecord');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDnsTextRecord', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
