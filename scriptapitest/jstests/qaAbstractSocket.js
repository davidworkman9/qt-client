assertIsNotConstructor(QAbstractSocket, 'QAbstractSocket');

var tmp = QAbstractSocket;
[
    'ShareAddress',
    'IPv4Protocol',
    'PauseNever',
    'ConnectionRefusedError',
    'LowDelayOption',
    'UnconnectedState',
    'TcpSocket'
].forEach(function (e) {
    assertHasProperty(QAbstractSocket, 'QAbstractSocket', e);
});
