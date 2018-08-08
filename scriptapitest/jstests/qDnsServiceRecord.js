/**
* Tests for QDnsServiceRecord
*/
var tmp = new QDnsServiceRecord()
    , properties = []
    , publicFunctions = [
        'name'
        , 'port'
        , 'priority'
        , 'swap'
        , 'target'
        , 'timeToLive'
        , 'weight'
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
