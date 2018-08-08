/**
* Tests for QByteArray obj
*/
var tmp = new QByteArray()
    , properties = []
    , publicFunctions = [
        'append'
        , 'at'
        , 'back'
        , 'begin'
        , 'capacity'
        , 'cbegin'
        , 'cend'
        , 'chop'
        , 'chopped'
        , 'clear'
        , 'constBegin'
        , 'constData'
        , 'constEnd'
        , 'contains'
        , 'count'
        , 'crbegin'
        , 'crend'
        , 'data'
        , 'end'
        , 'endsWith'
        , 'fill'
        , 'front'
        , 'indexOf'
        , 'insert'
        , 'isEmpty'
        , 'isNull'
        , 'lastIndexOf'
        , 'left'
        , 'leftJustified'
        , 'length'
        , 'mid'
        , 'prepend'
        , 'push_back'
        , 'push_front'
        , 'rawData'
        , 'rbegin'
        , 'remove'
        , 'rend'
        , 'repeated'
        , 'replace'
        , 'reserve'
        , 'resize'
        , 'right'
        , 'rightJustified'
        , 'setNum'
        , 'shrink_to_fit'
        , 'simplified'
        , 'size'
        , 'split'
        , 'squeeze'
        , 'startsWith'
        , 'swap'
        , 'toBase64'
        , 'toCFData'
        , 'toDouble'
        , 'toFloat'
        , 'toHex'
        , 'toInt'
        , 'toLong'
        , 'toLongLong'
        , 'toLower'
        , 'toNSData'
        , 'toPercentEncoding'
        , 'toRawCFData'
        , 'toRawNSData'
        , 'toShort'
        , 'toUpper'
        , 'trimmed'
        , 'truncate'
    ];

assertIsConstructor(QByteArray, 'QByteArray');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QByteArray', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
