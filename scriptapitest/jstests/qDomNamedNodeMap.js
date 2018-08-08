/**
* Tests for QDomNamedNodeMap
*/
var tmp = new QDomNamedNodeMap()
    , properties = []
    , publicFunctions = [
        'contains'
        , 'count'
        , 'isEmpty'
        , 'item'
        , 'length'
        , 'namedItem'
        , 'namedItemNS'
        , 'removeNamedItem'
        , 'removeNamedItemNS'
        , 'setNamedItem'
        , 'setNamedItemNS'
        , 'size'
    ];

assertIsConstructor(QDomNamedNodeMap, 'QDomNamedNodeMap');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDomNamedNodeMap', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
