/**
 * Created by pahaz_000 on 08/11/2015.
 */

function getRandomString() {
    return require('crypto').randomBytes(8).toString('hex');
}

module.exports = {
    'getRandomString': getRandomString
};
