#define DUCKDB_EXTENSION_MAIN

#include "hydro_duck_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/function/aggregate_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

#include "duckdb/function/aggregate_function.hpp"
#include "duckdb/function/function.hpp"

#include "functions/functions.hpp"


namespace duckdb {

inline void HydroDuckScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "HydroDuck "+name.GetString()+" 🐥");;
        });
}

static void LoadInternal(DatabaseInstance &instance) {
    // Register a scalar function
    auto hydro_duck_scalar_function = ScalarFunction("quack", {LogicalType::VARCHAR}, LogicalType::VARCHAR, HydroDuckScalarFun);
    ExtensionUtil::RegisterFunction(instance, hydro_duck_scalar_function);
}

void HydroDuckExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);

  Connection con(db);
  con.BeginTransaction();
  auto &catalog = Catalog::GetSystemCatalog(*con.context);
  hydro_duck::FirstScrooge::RegisterFunction(con, catalog);
  hydro_duck::NashSutcliffe::RegisterFunction(con, catalog);
  // hydro_duck::Aliases::Register(con, catalog);
  con.Commit();
}

std::string HydroDuckExtension::Name() {
	return "hydro_duck";
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void hydro_duck_init(duckdb::DatabaseInstance &db) {
    duckdb::DuckDB db_wrapper(db);
    db_wrapper.LoadExtension<duckdb::HydroDuckExtension>();
}

DUCKDB_EXTENSION_API const char *hydro_duck_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
