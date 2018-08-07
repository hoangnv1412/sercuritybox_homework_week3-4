#pragma once
#include "UserModel.h"
#include "databaseManager.h"
#include <string>
#include "sqlite3.h"
#include <ctime>

using namespace std;
class UserManager
{
public:
	UserManager();
	
	UserManager(string userName) {
		username = userName;
	};

	
	bool loginUser(string username, string pw) {
		/*int  rc;
		int res;
		openDb();
		string sql = "SELECT * FROM USER " \
			"WHERE USER.username = '" \
			+ username
			+ "' AND USER.password = '" + pw + "';";


		cout << "sql query : " << sql << endl;
		/* Execute SQL statement */
		/*rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
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
		return res;*/
		sqlite3_stmt *pSelectStatement = NULL;
		int iResult = SQLITE_ERROR;
		bool ret = true;
		openDb();
		string sql = "SELECT * FROM USER " \
			"WHERE USER.username = '" \
			+ username
			+ "' AND USER.password = '" + pw + "';";
		iResult = sqlite3_prepare_v2(db, sql.c_str(), -1, &pSelectStatement, 0);
		if ((iResult == SQLITE_OK) && (pSelectStatement != NULL))
		{
			iResult = sqlite3_step(pSelectStatement);
			//was found?
			if (iResult == SQLITE_DONE) {
				ret = false;
				sqlite3_clear_bindings(pSelectStatement);
				sqlite3_reset(pSelectStatement);
			}
			iResult = sqlite3_finalize(pSelectStatement);
		}
		return ret;
	}

	void addUser(string username, string pw) {
		openDb();
		string sql = "INSERT INTO USER (username,password) " \
			"VALUES('" + username + "','" + pw + "')";
		cout << "sql : " << sql << endl;
		int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "Insert success\n");
		}
		sqlite3_close(db);
	}

	void sendMess() {
		cout << "To : ...";
		char* receiver;
		receiver = new char;
		cin >> receiver;
		string sender = username;
		if (isExisted(receiver)) {
			if (!isBlocked(sender, receiver)) {
				char* content;
				content = new char;
				cout << "Content: ";
				cin >> content;

				//take real time
				time_t hientai = time(0);
				char* dt = ctime(&hientai);
				tm *gmtm = gmtime(&hientai);
				dt = asctime(gmtm);

				addMessToDB(sender,receiver,content,dt);
			}
			else cout << "Cannot send : user '" << receiver << "' is in your blocked list or has blocked you" << endl;

		}
		else cout << "Error : user '" << receiver << "' is not existed" << endl;
		fflush(stdin);

	}

	void showMess() {

	}

	void showDetailMess() {

	}

	void addFriend() {

	}

	void showFriendList() {

	}

	void Block() {

	}

	~UserManager();
private:

	int block_count;
	bool logined;
	string username;
	//DatabaseManager db_manager;
	
	const char* data = "Callback function called";
	sqlite3 * db;
	char *zErrMsg = 0;

	static int callback(void *data, int argc, char **argv, char **azColName) {
		int i;
		fprintf(stderr, "%s: ", (const char*)data);
		/*for (i = 0; i<argc; i++) {
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}*/
		printf("\n");
		return 0;
	}


	void addMessToDB(string sender, string receiver, string content, char* dt) {
		openDb();
		string sql = "INSERT INTO MESSAGE (sender,receiver,content,time) " \
			"VALUES('" + sender + "','" + receiver + "','" + content + "','" + dt +  "')";
		cout << "sql : " << sql << endl;
		int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "Insert success\n");
		}
		sqlite3_close(db);
	}


	void openDb() {
		int rc;
		rc = sqlite3_open("week4.db", &db);
		if (rc) {
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return;
		}
		else {
			
		}

	}

	int isExisted(string username) {
		bool check;
		int  rc;
		openDb();
		string sql = "SELECT * FROM USER " \
			"WHERE USER.ID = '" \
			+ username + "';";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			check = false;
		}
		else {
			check = true;
		}
		sqlite3_close(db);
		return check;
	}

	

	bool isBlocked(string sender, string receiver) {
		openDb();
		string sql = "SELECT * FROM BLOCK " \
			"WHERE ( BLOCK.blocker = '" \
			+ receiver + "' AND BLOCK.blocked = '" + sender \
			+ "' ) OR ( BLOCK.blocker = '" \
			+ sender + "' AND BLOCK.blocked = '" + receiver + "' );"; 

		/* Execute SQL statement */
		sqlite3_stmt *selectstmt = NULL;
		int iResult = SQLITE_ERROR;
		bool ret = true;
		int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &selectstmt, NULL);
		if ((result == SQLITE_OK) && (selectstmt != NULL))
		{
			if (sqlite3_step(selectstmt) == SQLITE_ROW)
			{
				// record found
			}
			else
			{
				ret =  false;
			}
		}
		sqlite3_finalize(selectstmt);
		return ret;
	}

	

};

