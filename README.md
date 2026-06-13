# Rain
## A drizzle-inspired implementation of a SQL query builder in C++

# Architecture
Rain uses an implementation of the builder design pattern in order to provide a nice API to build queries

Insert Example:
```c++
Rain db(sql); // Provider here

auto users = SqliteTable("users", std::tuple(Column<int>("user_id").primaryKey().notNull(), Column<std::string>("username").notNull(), Column<std::string>("email").unique()));

db.insert(users).values(std::tuple(1, "Wyatt", "wyatt@poke.com")).execute();
```

# Providers
Currently Sqlite3 is the only supported provider but I plan to implement PostgreSQL and maybe MySQL in the near future!
