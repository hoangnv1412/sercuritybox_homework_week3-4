#pragma once
#include <iostream>
#include "sqlite3.h"
#include "UserModel.h"
#include <string>
using namespace std;
class DatabaseManager
{
public:
	DatabaseManager();
	DatabaseManager(sqlite3 * Db) {
		db = Db;
	};
	void openDb() {
		int rc;
		rc = sqlite3_open("week4.db", &db);
		if (rc) {
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return;
		}
		else {
			fprintf(stderr, "Opened database successfully\n");
		}
		
	}


	int loginUser(string id, string pw) {
		int  rc;
		int res;
		string sql = "SELECT * FROM USER " \
			"WHERE USER.ID = '" \
			+ id
			+ "' AND USER.password = '" + pw + "';";
		char* query = &sql[0u];

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql.c_str(), select, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			res = 0;
		}
		else {
			fprintf(stdout, "OK\n");
			res = 1;
		}
		sqlite3_close(db);
		return res;
	}

	void addUser(string id, string pw) {
		string sql = "INSERT INTO USER (id,password) " \
			"VALUES('"  + id + "','" + pw +"')";
		cout << "sql : " << sql << endl;
		int rc = sqlite3_exec(db, sql.c_str(), select, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "Cannot insert\n");
		}
		sqlite3_close(db);
	}

	void closeDb() {
		sqlite3_close(db);
	}
	
	
	~DatabaseManager();
private:
	static int select(void *data, int argc, char **argv, char **azColName) {
		int i;
		fprintf(stderr, "%s: ", (const char*)data);
		for (i = 0; i<argc; i++) {
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
	}


	const char* data = "Callback function called";
	sqlite3 * db;
	char *zErrMsg = 0;
	

};

