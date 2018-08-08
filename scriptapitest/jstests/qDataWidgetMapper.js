/**
* Test the QDataWidgetMapper
*/
var tmp = new QDataWidgetMapper()
    , properties = [
        'currentIndex'
        , 'orientation'
        , 'submitPolicy'
    ]
    , publicFunctions = [
        'addMapping'
        , 'clearMapping'
        , 'currentIndex'
        , 'itemDelegate'
        , 'mapppedPropertyName'
        , 'mappedSelection'
        , 'mappedWidgetAt'
        , 'model'
        , 'orientation'
        , 'removeMapping'
        , 'rootIndex'
        , 'setItemDelegate'
        , 'setModel'
        , 'setOrientation'
        , 'setRootIndex'
        , 'setSubmitPolicy'
        , 'submitPolicy'
    ];

assertIsConstructor(QBuffer, 'QBuffer');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QBuffer', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
