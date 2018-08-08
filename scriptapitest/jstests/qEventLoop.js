/**
* Tests for the QEventLoop
*/
var tmp = new QEventLoop()
    , properties = []
    , publicFunctions = [
        'exec'
        , 'exit'
        , 'isRunning'
        , 'processEvents'
        , 'wakeUp'
    ];

assertIsConstructor(QEventLoop, 'QEventLoop');

properties.forEach(function (e) {
    assertHasProperty(tmp, 'QEventLoop', e);
});

publicFunctions.forEach(function (e) {
    assertIsFunction(tmp[e], e, 'e');
});
