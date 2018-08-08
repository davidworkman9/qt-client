[
    'Trigger',
    'NoRole'
].forEach(function (e) {
    assertHasProperty(QAction, 'QAction', e);
});

var pb   = new QPushButton();
print(pb);

assertIsConstructor(QAction, 'QAction', [pb]);
var act  = new QAction(pb);
[
    'activate',
    'data',
    'setData'
].forEach(function (e) {
    assertIsFunction(act[e], e);
});

var act2 = new QAction('TestQAction', pb);
assert(act2);
