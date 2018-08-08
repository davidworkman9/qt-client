/**
* Test the QWebEngineView object
*/
var tmp = new QWebEngineView()
    , properties = [
        'hasSelection'
        , 'icon'
        , 'iconUrl'
        , 'selectedText'
        , 'title'
        , 'url'
        , 'zoomFactor'
    ]
    , publicFunctions = [
        'findText'
        , 'history'
        , 'load'
        , 'page'
        , 'pageAction'
        , 'setContent'
        , 'setHtml'
        , 'setPage'
        , 'setUrl'
        , 'setZoomFactor'
        , 'settings'
    ]
    , url = 'http://www.example.com/'
    , qurl = new QUrl(url)
    , zoom = 2.7;

assertIsConstructor(QWebEngineView, 'QWebEngineView');

// Test URL
tmp.load(qurl);
assert(tmp.url == url, 'Expected ' + url + '. Recieved ' + tmp.url + '.');

// Test Zoom Factor
assert(tmp.zoomFactor === 1, 'Default zoomFactor should be 1');
tmp.setZoomFactor(zoom);
assert(Math.abs(tmp.zoomFactor - zoom) <= 0.0001, 'ZoomFactor should be ' + zoom + '. ZoomFactor is ' + tmp.zoomFactor + '.');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QWebEngineView', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
