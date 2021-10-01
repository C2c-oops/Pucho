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

vector<string> readFileInfo(string path)
{
	vector<string> infos;
	fstream file_handler(path.c_str());

	if (file_handler.fail())
	{
		cout << "\n\nError: Can't open the file\n\n";
		return infos;
	}

	string info;
	while (getline(file_handler, info))
	{
		if (info.size() == 0)
			continue;
		infos.push_back(info);
	}

	file_handler.close();
	return infos;
}

void writeFileLines(string path, vector<string> infos, bool append = true)
{
	auto status = ios::in | ios::out | ios::app;
	if (!append)
		status = ios::in | ios::out | ios::trunc; //overwrite data
	fstream file_handler(path.c_str(), status);

	if (file_handler.fail())
	{
		cout << "\n\nError: Can't open the file\n\n";
		return;
	}

	for (auto info : infos)
		file_handler << info << "\n";

	file_handler.close();
}
struct User
{
	int user_id;
	string user_name;
	string password;
	string name;
	string email;
	int allow_anonymous_question;

	vector<int> question_id_from_me;
	//thread question map(question id->list of question ids)
	map<int, vector<int>> question_thread_object_map;

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
		oss << user_id << ","
		    << user_name << ","
		    << password << ","
		    << name << ","
		    << email << ","
		    << allow_anonymous_question;
		return oss.str();
	}

	void printUserInfo()
	{
		cout << "User: " << user_id << ", " << user_name << ", " << password << "," << name << ", " << email << "\n";
	}
};

struct UsersManager
{
	map<string, User> user_object_map;
	User current_user;
	int last_id;

	UsersManager()
	{
		last_id = 0;
	}

	void loadDatabase()
	{
		last_id = 0;
		user_object_map.clear();

		vector<string> userInfos = readFileInfo("users.txt");
		for (auto &userInfo : userInfos)
		{
			User user(userInfo);
			user_object_map[user.user_name] = user;
			last_id = max(last_id, user.user_id);
		}
	}

	void updateDatabase(User &user)
	{
		string info = user.userInfoToString();
		vector<string> infos(1, info);
		writeFileLines("users.txt", infos);
	}

	void authentication()
	{
		vector<string> auth = {"Login", "Sign Up"};
		int choice = readMenu(auth);
		if (choice == 1)
			login();
		else
			signUp();
	}

	void login()
	{
		loadDatabase(); //if user added from parallel run
		while (true)
		{
			cout << "Enter user name & password: ";
			cin >> current_user.user_name >> current_user.password;

			if (user_object_map.find(current_user.user_name) == user_object_map.end())
			{
				cout << "\nInvalid user name or password. Try again\n\n";
				continue;
			}

			User valid_user = user_object_map[current_user.user_name];

			if (current_user.password != valid_user.password)
			{
				cout << "\nInvalid user name or password. Try again\n\n";
				continue;
			}
			current_user = valid_user;
			break;
		}
	}

	void signUp()
	{
		while (true)
		{
			cout << "Enter user name(should not include spaces): ";
			cin >> current_user.user_name;
			if (user_object_map.find(current_user.user_name) != user_object_map.end())
				cout << "User name already exists!(choose another)\n";
			else
				break;
		}
		cout << "Enter password: ";
		cin >> current_user.password;

		cout << "Enter name: ";
		cin >> current_user.name;

		cout << "Enter email: ";
		cin >> current_user.email;

		cout << "Allow anonymous questions? (0->No or 1->Yes): ";
		cin >> current_user.allow_anonymous_question;

		//for parallel sessions
		//same id will be assigned thats why
		current_user.user_id = ++last_id;
		user_object_map[current_user.user_name] = current_user;
	}

	void listUserNamesIds()
	{
		for (auto &user : user_object_map)
		{
			cout << "ID: " << user.second.user_id << "\t\tName: " << user.second.name << "\n";
		}
	}

	pair<int, int> readUserId()
	{
		int user_id;
		cout << "Enter user id or -1 to cancel: ";
		cin >> user_id;

		if (user_id == -1)
			return make_pair(-1, -1);
		for (auto &user : user_object_map)
		{
			if (user.second.user_id == user_id)
				return make_pair(user_id, user.second.allow_anonymous_question);
		}
		cout << "Invalid User Id. Try again\n";
		return readUserId();
	}
};

struct Question
{
	int question_id;
	//for supporting thread functionality
	//each question look for it's parent question
	//-1->No Parent->first question in the thread
	int parent_question_id;
	int from_user_id;
	int to_user_id;
	int is_anonymous_questions;
	string question_txt;
	string answer_txt; //if empty->not answered

	Question()
	{
		question_id = parent_question_id = from_user_id = to_user_id = -1;
		is_anonymous_questions = 1;
	}

	Question(string question_info)
	{
		vector<string> substrs = splitString(question_info);
		assert(substrs.size() == 7);

		question_id = toInt(substrs[0]);
		parent_question_id = toInt(substrs[1]);
		from_user_id = toInt(substrs[2]);
		to_user_id = toInt(substrs[3]);
		is_anonymous_questions = toInt(substrs[4]);
		question_txt = substrs[5];
		answer_txt = substrs[6];
	}

	string questionInfoToString()
	{
		ostringstream oss;
		oss << question_id << ","
		    << parent_question_id << ","
		    << from_user_id << ","
		    << to_user_id << ","
		    << is_anonymous_questions << ","
		    << question_txt << ","
		    << answer_txt;

		return oss.str();
	}

	void printQuestionsForUser()
	{
		string prefix = "";

		if (parent_question_id != -1)
			prefix = "\tThread: ";
		cout << prefix << "Question Id (" << question_id << ")";
		if (!is_anonymous_questions)
			cout << " from user id(" << from_user_id << ")";
		cout << "\tQuestion: " << question_txt << "\n";

		if (answer_txt != "")
			cout << prefix << "\tAnswer: " << answer_txt << "\n";
		cout << "\n";
	}

	void printQuestionsByUser()
	{
		cout << "Question Id (" << question_id << ")";
		if (!is_anonymous_questions)
			cout << " !AQ";
		cout << " to user id(" << to_user_id << ")";
		cout << "\t Question: " << question_txt;

		if (answer_txt != "")
			cout << "\tAnswer: " << answer_txt << "\n";
		else
			cout << "\tNot Answered yet!\n";
	}

	void printQuestionsForFeed()
	{
		if (parent_question_id != -1)
			cout << "Thread Parent Question ID (" << parent_question_id << ") ";
		cout << "Question Id (" << question_id << ")";
		if (!is_anonymous_questions)
			cout << " from user id(" << from_user_id << ")";
		cout << " To user id(" << to_user_id << ")";
		cout << "\t Question: " << question_txt << "\n";
		if (answer_txt != "")
			cout << "\tAnswer: " << answer_txt << "\n";
	}
};

struct QuestionManager
{
	//thread question map(question id->list of question ids)
	map<int, vector<int>> question_thread_object_map;

	map<int, Question> question_object_map;
	int last_id;

	QuestionManager()
	{
		last_id = 0;
	}

	void loadDatabase()
	{
		last_id = 0;
		question_thread_object_map.clear();
		question_thread_object_map.clear();

		vector<string> questionInfos = readFileInfo("questions.txt");
		for (auto &questionInfo : questionInfos)
		{
			Question question(questionInfo);
			last_id = max(last_id, question.question_id);
			question_object_map[question.question_id] = question;
			if (question.parent_question_id == -1)
				question_thread_object_map[question.question_id]
				    .push_back(question.question_id);
			else
				question_thread_object_map[question.parent_question_id]
				    .push_back(question.question_id);
		}
	}

	void updateDatabase()
	{
		vector<string> questionInfos;
		for (auto &questionInfo : question_object_map)
			questionInfos.push_back(questionInfo.second.questionInfoToString());
		writeFileLines("question.txt", questionInfos, false);
	}

	void fillUserQuestions(User &user)
	{
		user.question_id_from_me.clear();
		user.question_thread_object_map.clear();

		for (auto &question : question_thread_object_map) //question<int, vector<Question>>
		{
			for (auto &question_id : question.second) //vector<Question>
			{
				//getting question from map(from memory itslef) no copying
				Question &current_question = question_object_map[question_id];

				if (current_question.from_user_id == user.user_id)
				{
					user.question_id_from_me.push_back(current_question.question_id);
				}

				if (current_question.to_user_id == user.user_id)
				{
					if (current_question.parent_question_id == -1)
						user.question_thread_object_map[current_question.question_id]
						    .push_back(current_question.question_id);
					else
						user.question_thread_object_map[current_question.parent_question_id]
						    .push_back(current_question.question_id);
				}
			}
		}
	}

	void showQuestionsForUser(User &user)
	{
		cout << "\n";
		if (user.question_thread_object_map.size() == 0)
			cout << "No Questions for you.";
		for (auto &question : user.question_thread_object_map) //question<int, vector<Question>>
		{
			for (auto &question_id : question.second) //Vector<Question>
			{
				//getting question from map(from memory itslef) no copying
				Question &current_question = question_object_map[question_id];
				current_question.printQuestionsForUser();
			}
		}
		cout << "\n";
	}

	void showQuestionsByUser(User &user)
	{
		cout << "\n";
		if (user.question_id_from_me.size() == 0)
			cout << "No Questions";

		for (auto &question_id : user.question_id_from_me) //vector<Question>
		{
			//getting question from map(from memory itslef) no copying
			Question &current_question = question_object_map[question_id];
			current_question.printQuestionsByUser();
		}
		cout << "\n";
	}

	//used in answering question for user
	//can be any question (thread or not)
	int getAnyQuestionID(User &user)
	{
		int question_id;
		cout << "Enter Question id or -1 to cancel: ";
		cin >> question_id;

		if (question_id == -1)
			return -1;
		if (question_object_map.find(question_id) == question_object_map.end())
		{
			cout << "\nERROR: No question with such ID. Try again\n\n";
			return getAnyQuestionID(user);
		}
		Question &current_question = question_object_map[question_id];

		if (current_question.to_user_id != user.user_id)
		{
			cout << "\nERROR: Invalid question ID. Try again\n\n";
			return getAnyQuestionID(user);
		}
		return question_id;
	}

	//used for asking question on a specific thread for any user
	int getThreadQuestionID(User &user)
	{
		int question_id;
		cout << "For thread questions: Enter Question id or -1 for new question: ";
		cin >> question_id;

		if (question_id == -1)
			return -1;

		if (question_thread_object_map.find(question_id) == question_thread_object_map.end())
		{
			cout << "No thread question with such ID. Try again\n";
			return getThreadQuestionID(user);
		}
		return question_id;
	}

	void answerQuestion(User &user)
	{
		int question_id = getAnyQuestionID(user);

		if (question_id == -1)
			return;
		Question &current_question = question_object_map[question_id];
		current_question.printQuestionsForUser();

		if (current_question.answer_txt != "")
			cout << "\n Warning: Already answered. Answer will be updated\n";

		cout << "Enter answer: ";
		getline(cin, current_question.answer_txt);
		getline(cin, current_question.answer_txt);
	}

	void deleteQuestion(User &user)
	{
		int question_id = getAnyQuestionID(user);

		if (question_id == -1)
			return;

		vector<int> ids_to_remove; //to remove from question_object_map
		//if thread->remove all
		if (question_thread_object_map.find(question_id) != question_thread_object_map.end())
		{
			ids_to_remove = question_thread_object_map[question_id];
			question_thread_object_map.erase(question_id);
		}
		else
		{
			ids_to_remove.push_back(question_id);
			//getting which thread to delete
			for (auto &question : question_thread_object_map)
			{
				vector<int> &vec = question.second;
				for (int pos = 0; pos < (int)vec.size(); ++pos)
				{
					if (question_id == vec[pos])
					{
						vec.erase(vec.begin() + pos);
						break;
					}
				}
			}
		}
		for (auto id : ids_to_remove)
			question_object_map.erase(id);
	}

	void askQuestion(User &user, pair<int, int> to_user_pair)
	{
		Question question;
		if (!to_user_pair.second)
		{
			cout << "Note: Anonymous questions are not allowed for this user\n";
			question.is_anonymous_questions = 0;
		}
		else
		{
			cout << "Is annonymous questions?: (0 or 1): ";
			cin >> question.is_anonymous_questions;
		}

		question.parent_question_id = getThreadQuestionID(user);

		cout << "Enter question text: ";
		getline(cin, question.question_txt);
		getline(cin, question.question_txt);

		question.from_user_id = user.user_id;
		question.to_user_id = to_user_pair.first;

		//for parallel sessions
		//same id will be provide (wrong handling)
		question.question_id = ++last_id;
		question_object_map[question.question_id] = question;

		if (question.parent_question_id == -1)
			question_thread_object_map[question.question_id]
			    .push_back(question.question_id);
		else
			question_thread_object_map[question.parent_question_id]
			    .push_back(question.question_id);
	}

	void listFeed()
	{
		for (auto &question : question_object_map)
		{
			Question &current_question = question.second;

			if (current_question.answer_txt == "")
				continue;
			current_question.printQuestionsForFeed();
		}
	}
};

struct pucho_v1
{
	UsersManager users_manager;
	QuestionManager questions_manager;

	void loadDatabase(bool fill_user_questions = false)
	{
		users_manager.loadDatabase();
		questions_manager.loadDatabase();
		if (fill_user_questions) //first time access
			questions_manager.fillUserQuestions(users_manager.current_user);
	}
	void run()
	{
		loadDatabase();
		users_manager.authentication();
		questions_manager.fillUserQuestions(users_manager.current_user);

		vector<string> menu;
		menu.push_back("Print Questions asked to Me");
		menu.push_back("Print Questions asked by Me");
		menu.push_back("Answer Question");
		menu.push_back("Delete Question");
		menu.push_back("Ask Question");
		menu.push_back("List System Users");
		menu.push_back("Feed");
		menu.push_back("Logout");

		while (true)
		{
			int choice = readMenu(menu);
			loadDatabase(true);

			switch (choice)
			{
			case 1:
				cout << 1 << "pressed" << endl;
				questions_manager.showQuestionsForUser(users_manager.current_user);
				break;
			case 2:
				cout << 2 << "pressed" << endl;
				questions_manager.showQuestionsByUser(users_manager.current_user);
				break;
			case 3:
				cout << 3 << "pressed" << endl;
				questions_manager.answerQuestion(users_manager.current_user);
				questions_manager.updateDatabase();
				break;
			case 4:
				cout << 4 << "pressed" << endl;
				questions_manager.deleteQuestion(users_manager.current_user);
				questions_manager.fillUserQuestions(users_manager.current_user);
				questions_manager.updateDatabase();
				break;
			case 5:
				cout << 5 << "pressed" << endl;
				pair<int, int> to_user_pair = users_manager.readUserId();
				if (to_user_pair.first != -1)
				{
					questions_manager.askQuestion(users_manager.current_user, to_user_pair);
					questions_manager.updateDatabase();
				}
				break;
			case 6:
				cout << 6 << "pressed" << endl;
				users_manager.listUserNamesIds();
				break;
			case 7:
				cout << 7 << "pressed" << endl;
				questions_manager.listFeed();
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