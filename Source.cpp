#include <iostream>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

// Реализуйте функции и методы классов и при необходимости добавьте свои

class Date {
public:
	// конструкторы
	Date() {
		year = 0;
		month = 0;
		day = 0;
	}
	Date(const int new_year, const int new_month, const int new_day) {
		year = new_year;
		month = new_month;
		day = new_day;
	}
	// получение данных даты
	int GetYear() const {
		return year;
	};
	int GetMonth() const {
		return month;
	};
	int GetDay() const {
		return day;
	};
private:
	int year, month, day;
};

Date ReadDate(istream& stream) {
	stream.ignore(1);
	string date;

	getline(stream, date, ' ');
	stringstream date_stream(date);
	int year, month, day;
	char c;
	date_stream >> year >> c;
	if (c != '-' || !(date_stream >> month)) {
		throw runtime_error("Wrong date format: " + date);
	}
	date_stream >> c;
	if (c != '-' || !(date_stream >> day)) {
		throw runtime_error("Wrong date format: " + date);
	}
	if (date_stream >> c) {
		throw runtime_error("Wrong date format: " + date);
	}
	if (month > 12 || 1 > month) {
		throw runtime_error("Month value is invalid: " + to_string(month));
	}
	if (day > 31 || 1 > day) {
		throw runtime_error("Day value is invalid: " + to_string(day));
	}
	return Date(year, month, day);
};

bool operator<(const Date& left, const Date& right) {
	if (left.GetYear() != right.GetYear())
		return left.GetYear() < right.GetYear();
	else if (left.GetMonth() != right.GetMonth())
		return left.GetMonth() < right.GetMonth();
	else
		return left.GetDay() < right.GetDay();
};

ostream& operator<<(ostream& stream, const Date& date) {
	return stream << setfill('0') << setw(4) <<  date.GetYear() << '-' 
					<< setw(2) << date.GetMonth() << '-' << setw(2) << date.GetDay();
}

class Database {
public:
	// добавить новое событие event с датой date
	void AddEvent(const Date& date, const string& event) {
		Data[date].insert(event);
	}
	// удалить событие event из даты date
	bool DeleteEvent(const Date& date, const string& event) {
		if (Data.count(date) && Data.at(date).count(event)) {
			Data.at(date).erase(event);
			if (Data.at(date).size() == 0) {
				Data.erase(date);
			}
			return true;
		}
		else {
			return false;
		}
	};
	// удалить все события даты date
	int  DeleteDate(const Date& date) {
		if (Data.count(date)) {
			int n = Data.at(date).size();
			Data.erase(date);
			return n;
		}
		else {
			return 0;
		}
	};
	// найти и распечатать все события дня date
	void Find(const Date& date) const {
		if (Data.count(date)) {
			for (const auto& event : Data.at(date)) {
				cout << event << endl;
			}
		}
	};
	// распечатать все события
	void Print() const {
		for (const auto& item : Data) {
			for (auto event : item.second) {
				cout << item.first << " " << event << endl;
			}
		}
	};
private:
	map<Date, set<string>> Data;
};

int main() {
	Database db;

	string command;
	//ifstream input("input.txt");
	while (getline(cin, command)) {
		// Считайте команды с потока ввода и обработайте каждую
		stringstream stream(command);
		string command_name;
		stream >> command_name;
		if (command_name == "")
			continue;
		if (command_name == "Add" || command_name == "Del" || command_name == "Find") {
			Date date;
			try {
				date = ReadDate(stream);
			}
			catch (exception& ex) {
				cout << ex.what() << endl;
				return 0;
			}

			if (command_name == "Add") {
				string event;
				stream >> event;
				db.AddEvent(date, event);
			}
			else if (command_name == "Del") {
				string event;
				if (stream >> event) {
					if (db.DeleteEvent(date, event)) {
						cout << "Deleted successfully" << endl;
					}
					else {
						cout << "Event not found" << endl;
					}
				}
				else {
					int n = db.DeleteDate(date);
					cout << "Deleted " << n << " events" << endl;
				}
			}
			else {
				db.Find(date);
			}
		} 
		else if (command_name == "Print") {
			db.Print();
		}
		else {
			cout << "Unknown command: " << command_name << endl;
			return 0;
		}
	}
	return 0;
}