#include <iostream>
#include <string>
#include <tuple>

extern "C" {
	#include <sqlite3.h>
}
#include "rain.h"


class Rain {
	public:
		Rain(sqlite3* provider);
		Rain& select();

		template<typename... Cols>
		InsertQuery<Cols...> insert(SqliteTable<Cols...> table);
		template<typename... Cols>
		SelectQuery<Cols...> from(SqliteTable<Cols...> table);
//		Rain& where(std::string expression);

	private:
		sqlite3* provider_;

};

Rain::Rain(sqlite3* provider): provider_(provider){}

Rain& Rain::select() {
	return *this;
}

template<typename... Cols>
InsertQuery<Cols...> Rain::insert(SqliteTable<Cols...> table) {
	return InsertQuery<Cols...>(provider_, table);
}

template<typename... Cols>
SelectQuery<Cols...> Rain::from(SqliteTable<Cols...> table) {
	return SelectQuery<Cols...>();
}

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
