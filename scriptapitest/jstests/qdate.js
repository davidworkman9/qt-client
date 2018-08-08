/**
* Tests for QDate
*/
var tmp = new QDate()
    , properties = []
    , publicFunctions = [
        'addDays'
        , 'addYears'
        , 'day'
        , 'dayOfWeek'
        , 'dayOfYear'
        , 'daysTo'
        , 'getDate'
        , 'isNull'
        , 'isValid'
        , 'month'
        , 'setDate'
        , 'toJulianDay'
        , 'toString'
        , 'weekNumber'
        , 'year'
    ];

assertIsConstructor(QDate, 'QDate');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDate', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
