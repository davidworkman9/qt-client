/**
* Tests for QDnsLookup
*/
var tmp = new QDnsLookup()
    , properties = [
        'error'
        , 'errorString'
        , 'name'
        , 'nameserver'
        , 'type'
    ]
    , publicFunctions = [
        'canonicalNameRecords'
        , 'hostAddressRecords'
        , 'isFinished'
        , 'mailExchangeRecords'
        , 'nameServerRecords'
        , 'pointerRecords'
        , 'serviceRecords'
        , 'setName'
        , 'setNameserver'
        , 'setType'
        , 'textRecords'
    ];

assertIsConstructor(QDnsLookup, 'QDnsLookup');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDnsLookup', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
