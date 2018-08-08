/**
* Tests for QDir
*/
var tmp = new QDir()
    , properties = []
    , publicFunctions = [
        'cd'
        ,'cdUp'
        , 'count'
        , 'dirName'
        , 'entryInfoList'
        , 'entryInfoList'
        , 'entryList'
        , 'exists'
        , 'filePath'
        , 'filter'
        , 'isAbsolute'
        , 'isEmpty'
        , 'isReadable'
        , 'isRelative'
        , 'isRoot'
        , 'makeAbsolute'
        , 'mkdir'
        , 'mkpath'
        , 'nameFilters'
        , 'path'
        , 'refresh'
        , 'relativeFilePath'
        , 'remove'
        , 'removeRecursively'
        , 'rename'
        , 'rmdir'
        , 'rmpath'
        , 'setFilter'
        , 'setNameFilters'
        , 'setPath'
        , 'setSorting'
        , 'sorting'
        , 'swap'
    ];

assertIsConstructor(QDir, 'QDir');

// Test the properties
properties.forEach(function(property) {
    assertHasProperty(tmp, 'QDir', property);
});

// Test the public methods
publicFunctions.forEach(function(func) {
    assertIsFunction(tmp[func], func);
});
