/**
* Tests for QDockWidget
*/
var tmp = new QDockWidget()
    , properties = [
        'allowedAreas'
        , 'features'
        , 'floating'
        , 'windowTitle'
    ]
    , publicFunctions = [
        'isAreaAllowed'
        , 'isFloating'
        , 'setAllowedAreas'
        , 'setFeatures'
        , 'setFloating'
        , 'setTitleBarWidget'
        , 'setWidget'
        , 'titleBarWidget'
        , 'toggleViewAction'
        , 'widget'
    ];

assertIsConstructor(QDockWidget, 'QDockWidget');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDockWidget', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
