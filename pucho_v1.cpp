#include <bits/stdc++.h>

using namespace std;

vector<string> splitString(string str, string separator = ",")
{
	vector<string> strs;
	int position = 0;
	string subStr;
	while ((position = (int)str.find(separator)) != -1)
	{
		subStr = str.substr(0, position);
		strs.push_back(subStr);
		str.erase(0, position + separator.length());
	}
	strs.push_back(str);
	return strs;
}

int readInt(int low, int high)
{
	cout << "\nEnter number in range " << low << " - " << high << ": ";
	int value;
	cin >> value;
	if (low <= value && value <= high)
		return value;
	cout << "ERROR: invalid number...Try again\n";
	return readInt(low, high);
}

int toInt(string str)
{
	istringstream iss(str);
	int num;
	iss >> num;
	return num;
}

int readMenu(vector<string> &choices)
{
	cout << "\nMenu:\n";
	for (int choice = 0; choice < choices.size(); choice++)
	{
		cout << "\t" << choice + 1 << ": " << choices[choice] << "\n";
	}
	return readInt(1, choices.size());
}

struct User
{
	int user_id;
	string user_name;
	string password;
	string name;
	string email;
	int allow_anonymous_question;

	User()
	{
		user_id = allow_anonymous_question = -1;
	}

	User(string user_info)
	{
		vector<string> substrs = splitString(user_info);
		assert(substrs.size() == 6);
		user_id = toInt(substrs[0]);
		user_name = substrs[1];
		password = substrs[2];
		name = substrs[3];
		email = substrs[4];
		allow_anonymous_question = toInt(substrs[5]);
	}

	string userInfoToString()
	{
		ostringstream oss;
		oss << user_id << "," << user_name << "," << password << "," << name << "," << email << "," << allow_anonymous_question;
		return oss.str();
	}

	void printUserInfo()
	{
		cout << "User: " << user_id << ", " << user_name << ", " << password << "," << name << ", " << email << "\n";
	}
};

struct pucho_v1
{
	void run()
	{
		vector<string> menu;
		menu.push_back("Print Questions To Me");
		menu.push_back("Print Questions From Me");
		menu.push_back("Answer Question");
		menu.push_back("Delete Question");
		menu.push_back("Ask Question");
		menu.push_back("List System Users");
		menu.push_back("Feed");
		menu.push_back("Logout");

		while (true)
		{
			int choice = readMenu(menu);

			switch (choice)
			{
			case 1:
				cout << 1 << "pressed" << endl;
				break;
			case 2:
				cout << 2 << "pressed" << endl;
				break;
			case 3:
				cout << 3 << "pressed" << endl;
				break;
			case 4:
				cout << 4 << "pressed" << endl;
				break;
			case 5:
				cout << 5 << "pressed" << endl;
				break;
			case 6:
				cout << 6 << "pressed" << endl;
				break;
			case 7:
				cout << 7 << "pressed" << endl;
				break;
			case 8:
				cout << 8 << "pressed" << endl;
				break;
			default:
				break;
			}
		}
		run();
	}
};

int main()
{
	//developing new project
	pucho_v1 service;
	service.run();

	return 0;
}