var error
    , info
    , log
    , warn
    , print
    , tmp;

assertIsFunction(print, 'print');
print('print print');
assertIsFunction(error, 'error');
error('print error');
assertIsFunction(info,  'info');
info ('print info');
assertIsFunction(log,   'log');
log('print log');
assertIsFunction(warn,  'warn');
warn('print warn');
