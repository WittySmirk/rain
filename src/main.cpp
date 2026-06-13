#include <iostream>
#include <string>
#include <tuple>

extern "C" {
	#include <sqlite3.h>
}
#include "rain.h"

int main() {
	sqlite3* sql;
	int rc; rc = sqlite3_open("mydb.db", &sql);

	if (rc) {
		std::cerr << "Can't open database " << sqlite3_errmsg(sql) << std::endl;
		return -1;
	}
	std::cout << "Created database" << std::endl;

	Rain db(sql);

	auto users = SqliteTable("users", std::tuple(Column<int>("user_id").primaryKey().notNull(), Column<std::string>("username").notNull(), Column<std::string>("email").unique()));

	db.insert(users).values(std::tuple(1, "Wyatt", "wyatt@poke.com")).execute();

	sqlite3_close(sql);

	return 0;
}
