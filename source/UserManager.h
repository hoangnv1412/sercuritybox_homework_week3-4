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
		openDb();
		string sql = "SELECT MESSAGE.* FROM MESSAGE,USER WHERE USER.username = '" + username +"';";
			
		cout << "sql : " << sql << endl;
		int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
		}
		sqlite3_close(db);
	}

	void showDetailMess() {

	}

	void addFriend() {
		openDb();
		string frien;
		
		cout << "Enter username: ";
		cin >> frien;
		bool isEx = isExisted(frien);
		bool isBl = isBlocked(username, frien);
		bool isFr = isFriend(username, frien);

		if (!isEx) cout << "ERROR : user does not exist" << endl;
		else if (isBl) cout << "ERROR : block cmnr" << endl;
		else if (isFr) cout << "You are already friend" << endl;
		if (isEx && !isBl && !isFr) {
			string sql = "INSERT INTO FRIEND (user1,user2) " \
				"VALUES('" + username + "','"  + frien + "')";
			cout << "sql : " << sql << endl;
			int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			else {
				fprintf(stdout, "Insert success\n");
			}
			cout << "Add friend success" << endl;
		}
		
		sqlite3_close(db);
	}

	void showFriendList() {
		openDb();
		string sql = "SELECT friend.* FROM friend,user WHERE USER.username = '" + username + "';";

		cout << "sql : " << sql << endl;
		int rc = sqlite3_exec(db, sql.c_str(), callback_frl, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
		}
		cout << "\n\n";
		sqlite3_close(db);
	}

	void Block() {
		string victim;
		openDb();
		cout << "Enter name you want to block : ";
		cin >> victim;
		clear_buffer();
		bool isEx = isExisted(victim);
		bool isBl = isBlocked(username, victim);
		bool isFr = isFriend(username, victim);
		if (!isEx) cout << "ERROR : user does not exist" << endl;
		else if (isBl) cout << "ERROR : Username is already blocked" << endl;
		else if (isFr) cout << "Username is in your friendlist" << endl;
		if (isEx && !isBl) {
			cout << "Are you sure you want to block this person?\nEnter Y for Yes, N for No : ";
			char ch = getchar();
			while ((ch != 'Y') && (ch != 'N') && (ch != 'n') && (ch != 'y')) {
				cout << "Wrong syntax. Please try again" << endl;
				cout << "Enter Y for Yes, N for No : ";
				ch = getchar();
			}

			if ((ch == 'N') || (ch == 'n')) {
				cout << "Okay\n";
				return;
			}
			else {
				BlockExe(victim);
				if (isFr) Unfriend(username, victim);
			}
			
		}
		sqlite3_close(db);
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

	static int callback_frl(void *data, int argc, char **argv, char **azColName) {
		int i;
		fprintf(stderr, "id\tuser1\tuser2\n", (const char*)data);
		for (i = 0; i<argc; i++) {
			printf("%s\t", argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
	}

	static int callback(void *data, int argc, char **argv, char **azColName) {
		int i;
		fprintf(stderr, "%s: ", (const char*)data);
		for (i = 0; i<argc; i++) {
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
	}


	void addMessToDB(string sender, string receiver, string content, char* dt) {
		openDb();
		string sql = "INSERT INTO MESSAGE (sender,receiver,content,time) " \
			"VALUES('" + sender + "','" + receiver + "','" + content + "','" + dt +  "')";
		//cout << "sql : " << sql << endl;
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

	void BlockExe(string victim) {
		
		string sql = "INSERT INTO BLOCK (blocker,blocked) " \
			"VALUES('" + username + "','" + victim + "')";
		//cout << "sql : " << sql << endl;
		int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			
		}
		
	}

	void Unfriend(string username, string victim) {
		
		string sql = "DELETE FROM FRIEND " \
			"WHERE ( FRIEND.user1 = '" \
			+ username + "' AND FRIEND.user2 = '" + victim \
			+ "' ) OR ( FRIEND.user1 = '" \
			+ victim + "' AND FRIEND.user2 = '" + username + "' );";

		//cout << "sql : " << sql << endl;
		int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			
		}
		
	}

	int isExisted(string username) {
		openDb();
		string sql = "SELECT * FROM USER " \
			"WHERE USER.ID = '" \
			+ username + "';";

		sqlite3_stmt *pSelectStatement = NULL;
		int iResult = SQLITE_ERROR;
		bool ret = false;

		iResult = sqlite3_prepare_v2(db, sql.c_str(), -1, &pSelectStatement, 0);
		if ((iResult == SQLITE_OK) && (pSelectStatement != NULL))
		{
			iResult = sqlite3_step(pSelectStatement);
			//was found?
			if (iResult == SQLITE_DONE) {
				ret = true;
				sqlite3_clear_bindings(pSelectStatement);
				sqlite3_reset(pSelectStatement);
			}
			iResult = sqlite3_finalize(pSelectStatement);
		}
		return ret;
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
		sqlite3_close(db);
		return ret;
	}

	bool isFriend(string user1, string user2) {
		openDb();
		string sql = "SELECT * FROM FRIEND " \
			"WHERE ( FRIEND.user1 = '" \
			+ user1 + "' AND FRIEND.user2 = '" + user2 \
			+ "' ) OR ( FRIEND.user1 = '" \
			+ user2 + "' AND FRIEND.user2 = '" + user1 + "' );";

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
				ret = false;
			}
		}
		sqlite3_close(db);
		return ret;
	}

	void clear_buffer() {
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
	}

};

