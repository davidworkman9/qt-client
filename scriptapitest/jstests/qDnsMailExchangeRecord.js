/**
* Tests for QDnsMailExchangeRecord
*/
var tmp = new QDnsMailExchangeRecord()
    , properties = []
    , publicFunctions = [
        'exchange'
        , 'name'
        , 'preference'
        , 'swap'
        , 'timeToLive'
    ];

assertIsConstructor(QDnsMailExchangeRecord, 'QDnsMailExchangeRecord');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDnsMailExchangeRecord', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
