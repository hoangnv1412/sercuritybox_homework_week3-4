// testSQLite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "sqlite3.h"
#include "UserModel.h"
#include "databaseManager.h"
#include "UserManager.h"
#include <string>

using namespace std;

void clear_buffer() {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}

void login() ;
void signup() ;
void bai3() {};
void bai4() {};
void bai5() {};
void bai6() {};

int main(int argc, char* argv[])
{
	//DatabaseManager Db;
	
	UserModel User("1", "abc");
	cout << User.getId() << "  " << User.getPw() << "\n" << endl;
	// Db.openDb();
	/* Execute SQL statement */
	int init=0;
	while (init != 3) {
		printf("WEEK 4\n\n");
		printf("1.SIGN IN \n");
		printf("2.SIGN UP \n");
		printf("3. Exit application\n");
		printf("Enter a number (1-3): ");
		cin >> init;
		clear_buffer();
		switch (init) {
		case 1: login(); break;
		case 2: signup(); break;
		case 3: printf("Goodbye."); break;
		default: {
			printf("Wrong syntax. Please try again");
			break;
		}
		}
		printf("\n\n\n");
	}
		
	//UserManager userManager(User);
	

	return 0;
}

void login() {
	string username, pw;
	
	cout << "Enter username" << endl;
	cin >> username;
	cout << "Enter pw" << endl;
	cin >> pw;
	UserManager userManager(username);
	if (userManager.loginUser(username, pw) ) {
		int init2=0;
		while (init2 != 7) {
			printf("MENU\n\n");
			printf("1. Send Messages \n");
			printf("2. Message \n");
			printf("3. View Detail Message\n");
			printf("4. Add friends \n");
			printf("5. Show friend list \n");
			printf("6. Block\n");
			printf("7. Log out \n");
			printf("Enter a number (1-7): ");
			cin >> init2;
			clear_buffer();
			switch (init2) {
			case 1: userManager.sendMess(); break;
			case 2: userManager.showMess(); break;
			case 3: userManager.showDetailMess(); break;
			case 4: userManager.addFriend(); break;
			case 5: userManager.showFriendList(); break;
			case 6: userManager.Block();
			case 7: break;
			default: {
				printf("Wrong syntax. Please try again");
				break;
			}
			}

		}
	}
	else cout << "Wrong id or password" << endl;
}
void signup() {
	string id, pw, rewrite_pw;
	UserManager userManager;
	cout << "Enter Id" << endl;
	cin >> id;
	cout << "Enter pw" << endl;
	cin >> pw;
	while (rewrite_pw != pw) {
		cout << "Rewrite pw" << endl;
		cin >> rewrite_pw;
	}
	userManager.addUser(id,pw);

}