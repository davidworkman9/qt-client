/**
* Tests for QDnsHostAddressRecord
*/
var tmp = new QDnsHostAddressRecord()
    , properties = []
    , publicFunctions = [
        'name'
        , 'swap'
        , 'timeToLive'
        , 'value'
    ];

assertIsConstructor(QDnsHostAddressRecord, 'QDnsHostAddressRecord');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDnsHostAddressRecord', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
