/**
* Tests for QDnsDomainNameRecord
*/
var tmp = new QDnsDomainNameRecord()
    , properties = []
    , publicFunctions = [
        'name'
        , 'swap'
        , 'timeToLive'
        , 'value'
    ];

assertIsConstructor(QDnsDomainNameRecord, 'QDnsDomainNameRecord');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDnsDomainNameRecord', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
