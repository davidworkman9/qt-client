assertIsConstructor(orReport, 'orReport');
var tmp = new orReport();

assertHasProperty(tmp, 'orReport', 'beginMultiPrint');
assertHasProperty(tmp, 'orReport', 'endMultiPrint');

tmp = new orReport('Alignment');

[
    'backgroundAlignment',
    'backgroundImage',
    'isValid',
    'print',
    'reportError',
    'watermarkText'
].forEach(function (e) {
    assertIsFunction(tmp[e], e);
});
