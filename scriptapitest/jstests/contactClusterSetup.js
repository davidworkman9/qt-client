try {
    var tmp = ContactCluster;
    if (! tmp) throw new Error('ContactCluster should be exposed in widgets, not scriptapi');
} catch (e) {
    if (e instanceof ReferenceError)
        true;
    else
        throw e;
}
