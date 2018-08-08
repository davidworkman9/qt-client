/**
* Test the QWebEnginePage object
*/
var tmp = new QWebEnginePage()
    , properties = [
        'audioMuted'
        , 'backgroundColor'
        , 'contentsSize'
        , 'hasSelection'
        , 'icon'
        , 'iconUrl'
        , 'recentlyAudible'
        , 'requestedUrl'
        , 'scrollPosition'
        , 'selectedText'
        , 'title'
        , 'url'
        , 'zoomFactor'
    ]
    , publicFunctions = [
        'action'
        , 'contextMenuData'
        , 'createStandardContextMenu'
        , 'download'
        , 'findText'
        , 'history'
        , 'isAudioMuted'
        , 'print'
        , 'profile'
        , 'replaceMisspelledWord'
        , 'save'
        , 'scripts'
        , 'setAudioMuted'
        , 'setBackgroundColor'
        , 'setContent'
        , 'setFeaturePermission'
        , 'setHtml'
        , 'setUrl'
        , 'setView'
        , 'setWebChannel'
        , 'setZoomFactor'
        , 'settings'
        , 'toHtml'
        , 'triggerAction'
        , 'view'
        , 'webChannel'
    ]
    , url = 'http://www.example.com/'
    , qurl = new QUrl(url);

// Test constructor
assertIsConstructor(QWebEnginePage, 'QWebEnginePage');

// Test URL
tmp.setUrl(qurl);
assert(tmp.url == url, 'URL should be ' + url + '. URL was ' + tmp.url);

// Test misc ENUMs
assert(QWebEnginePage.MediaVideoCapture === 3, 'MediaVideoCapture should be 3');
assert(QWebEnginePage.FileSelectOpen === 0, 'FileSelectOpen should be 0');
assert(QWebEnginePage.FindCaseSensitively === 2, 'FindCaseSensitively should be 2');
assert(QWebEnginePage.WarningMessageLevel === 1, 'WarningMessageLevel should be 1');
assert(QWebEnginePage.NavigationTypeTyped === 1, 'NavigationTypeTyped should be 1');
assert(QWebEnginePage.NavigationTypeReload === 4, 'NavigationTypeReload should be 4');
assert(QWebEnginePage.PermissionDeniedByUser === 2, 'PermissionDeniedByUser should be 2');
assert(QWebEnginePage.CrashedTerminationStatus === 2, 'CrashedTerminationStatus should be 2');
assert(QWebEnginePage.KilledTerminationStatus === 3, 'KilledTerminationStatus should be 3');
assert(QWebEnginePage.Stop === 2, 'Stop should be 2');
assert(QWebEnginePage.PasteAndMatchStyle === 11, 'PasteAndMatchStyle should be 11');
assert(QWebEnginePage.CopyImageUrlToClipboard === 18, 'CopyImageUrlToClipboard should be 18');
assert(QWebEnginePage.DownloadLinkToDisk === 16, 'DownloadLinkToDisk should be 16');
assert(QWebEnginePage.ToggleBold === 33, 'ToggleBold should be 33');
assert(QWebEnginePage.AlignJustified === 40, 'AlignJustified should be 40');
assert(QWebEnginePage.WebBrowserBackgroundTab === 3, 'WebBrowserBackgroundTab should be 3');

// Test Muted
assert(tmp.isAudioMuted() === false, 'isAudioMuted should be false');
tmp.setAudioMuted(true);
assert(tmp.isAudioMuted() === true, 'isAudioMuted should be true');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QWebEnginePage', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
