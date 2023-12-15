import duckdb
import os
import pytest

# Get a fresh connection to DuckDB with the hydro_duck extension binary loaded
@pytest.fixture
def duckdb_conn():
    extension_binary = os.getenv('HYDRO_DUCK_EXTENSION_BINARY_PATH')
    if (extension_binary == ''):
        raise Exception('Please make sure the `HYDRO_DUCK_EXTENSION_BINARY_PATH` is set to run the python tests')
    conn = duckdb.connect('', config={'allow_unsigned_extensions': 'true'})
    conn.execute(f"load '{extension_binary}'")
    return conn

def test_hydro_duck(duckdb_conn):
    duckdb_conn.execute("SELECT hydro_duck('Sam') as value;");
    res = duckdb_conn.fetchall()
    assert(res[0][0] == "HydroDuck Sam 🐥");

def test_hydro_duck_openssl_version_test(duckdb_conn):
    duckdb_conn.execute("SELECT hydro_duck_openssl_version('Michael');");
    res = duckdb_conn.fetchall()
    assert(res[0][0][0:51] == "HydroDuck Michael, my linked OpenSSL version is OpenSSL");