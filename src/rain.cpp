#include "rain.h"

template<>
void bindValue<int>(sqlite3_stmt* stmt, int index, const int& value) {
    sqlite3_bind_int(stmt, index, value);
}

template<>
void bindValue<std::string>(sqlite3_stmt* stmt, int index, const std::string& value) {
    sqlite3_bind_text(stmt, index, value.c_str(), value.size(), SQLITE_TRANSIENT);
}

Rain::Rain(sqlite3* provider): provider_(provider){}

Rain& Rain::select() {
	return *this;
}
