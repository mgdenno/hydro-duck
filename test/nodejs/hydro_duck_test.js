var duckdb = require('../../duckdb/tools/nodejs');
var assert = require('assert');

describe(`hydro_duck extension`, () => {
    let db;
    let conn;
    before((done) => {
        db = new duckdb.Database(':memory:', {"allow_unsigned_extensions":"true"});
        conn = new duckdb.Connection(db);
        conn.exec(`LOAD '${process.env.HYDRO_DUCK_EXTENSION_BINARY_PATH}';`, function (err) {
            if (err) throw err;
            done();
        });
    });

    it('hydro_duck function should return expected string', function (done) {
        db.all("SELECT hydro_duck('Sam') as value;", function (err, res) {
            if (err) throw err;
            assert.deepEqual(res, [{value: "HydroDuck Sam 🐥"}]);
            done();
        });
    });

    it('hydro_duck_openssl_version function should return expected string', function (done) {
        db.all("SELECT hydro_duck_openssl_version('Michael') as value;", function (err, res) {
            if (err) throw err;
            assert(res[0].value.startsWith('HydroDuck Michael, my linked OpenSSL version is OpenSSL'));
            done();
        });
    });
});