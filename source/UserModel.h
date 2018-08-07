#pragma once
#include <iostream>
#include <cstring>
using namespace std;
class UserModel
{
private:
	string id;
	string pw;
	string name;
	string sex;
	string address;
public:
	UserModel() ;
	UserModel(string Id, string Pw) {
		this->id = Id;
		this->pw = Pw;
	};

	string getName() {
		return name;
	}
	string getId() {
		return id;
	}
	string getPw() {
		return pw;
	}
	void setName(string name) {
		this->name = name;
	}
	
	~UserModel();
};


