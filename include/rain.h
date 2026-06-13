#pragma once

#include <iostream>
#include <string>
#include <tuple>

extern "C" {
	#include <sqlite3.h>
}

enum RowType {
	INT,
	STRING,
	PRIMKEY,
};

template<typename T>
void bindValue(sqlite3_stmt* stmt, int index, const T& value);


template<typename T>
class Column {
	public:
		Column(std::string name): name_(name) {}
		Column& primaryKey() { type_ = PRIMKEY; return *this; }
		Column& notNull() { notNull_ = true; return *this; }
		Column& unique() { unique_ = true; return *this; }
		Column& type(RowType type) { type_ = type; return *this; }
		using value_type = T;
	const std::string& name() const { return name_; }
	private:
		std::string name_;
		bool notNull_ = false;
		bool unique_ = false;
		RowType type_;
};


template<typename... Cols>
class SqliteTable {
	public:
		SqliteTable(std::string name, std::tuple<Cols...> columns): name_(name), columns_(columns) {}
		const std::tuple<Cols...>& getColumns() const { return columns_; }
		const std::string& getName() const { return name_; }
	private:
		std::tuple<Cols...> columns_;
		std::string name_;
};


template<typename... Cols>
class SelectQuery {
	public:
		SelectQuery(sqlite3* provider, SqliteTable<Cols...> table): provider_(provider), table_(table) {}
		using rowType = std::tuple<typename Cols::value_type...>;
		SelectQuery& where() { return *this; }
	private:
		sqlite3* provider_;
		SqliteTable<Cols...> table_;
		// expressions
};

template<typename... Cols>
class InsertQuery {
	public:
		InsertQuery(sqlite3* provider, SqliteTable<Cols...> table): provider_(provider), table_(table) {}
		using rowType = std::tuple<typename Cols::value_type...>;
		InsertQuery& values(rowType values) { values_ = values; return *this; }

		void execute() {
			std::string sql = result();
			sqlite3_stmt* stmt;
			sqlite3_prepare(provider_, sql.c_str(), -1, &stmt, nullptr);

			std::size_t index = 1;
			std::apply([&](auto&... args) {
			    (bindValue(stmt, index++, args), ...);
			}, values_);

			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	private:
		std::string result() {
			std::string cols;
			std::apply([&](auto&&... col) {
				((cols += col.name() + ", "), ...);
			}, table_.getColumns());
			if (!cols.empty()) cols.resize(cols.size() - 2);

			std::string placeholders;
			std::apply([&](auto&&... col) {
				(..., ((void)col, (placeholders += "?, ")));
			}, table_.getColumns());
			if (!placeholders.empty()) placeholders.resize(placeholders.size() - 2);

			std::string sql = "INSERT INTO " + table_.getName() + " (" + cols + ") VALUES (" + placeholders + ");";

			std::cout << sql << std::endl;
			return sql;
		}
		sqlite3* provider_;
		SqliteTable<Cols...> table_;
		rowType values_;
};
