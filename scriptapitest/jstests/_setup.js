/**
* Assertion Functions
*/
function assertIsFunction(f, name) {
  // this may not be the best because if f returns false then this is returned
  // even if it's a valid function
  // if (!f)
  //   throw new Error(name + ' is falsey');

  if (typeof f != 'function')
    throw new Error(name + ' is not a function');
}

function assertIsConstructor(f, name, args) {
    assertIsFunction(f, name);
    var tmp;
    var args;
    if (!args || !args.length)
        tmp = f();
    else
        switch (args.length) {
            case 1:
                tmp = f(args[0]);
                break;
            case 2:
                tmp = f(args[0], args[1]);
                break;
            case 3:
                tmp = f(args[0], args[1], args[2]);
                break;
            default:
                tmp = f(args[0], args[1], args[2]);
                print('ignoring more than 3 args');
        }
}

function assertIsNotConstructor(f, name) {
    try {
        var tmp = new f();
        throw new Error(name + ' appears to be a constructor');
    } catch (e) {}
}

function assertHasProperty(obj, name, prop) {
    if (!obj.hasOwnProperty(prop)) {
        throw new Error(prop + ' is not a property of ' + name);
    }
}

function assert(condition, message) {
    if (!condition) {
        throw Error('Assert failed' + (typeof message !== 'undefined' ? ': ' + message : ''));
    }
}
