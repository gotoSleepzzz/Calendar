#include "Calendar.h"

Calendar* Calendar::_instance = NULL;
Date* Calendar::_now = NULL;

Calendar::Calendar() {
	_now = getTimeNow();
}

Date* Calendar::getTimeNow() {
	time_t now = time(0);
	tm* detail = localtime(&now);
	_now = new Date;

	_now->day = detail->tm_mday;
	_now->month = detail->tm_mon + 1;
	_now->year = detail->tm_year + 1900;

	return _now;
}

void BackGroundColor(WORD color) {
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	color <<= 4;
	wAttributes &= 0xff0f;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void TextColor(WORD color) {
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

Calendar* Calendar::getObject() {
	if (_instance == NULL) {
		_instance = new Calendar();
	}
	return _instance;
}

void Calendar::printCalendar(const Date& d) {
	cout << "\n\n\n\t\t\t\t\t\t " << left << setw(10) << MON[d.month - 1] << right << setw(10) << d.year << endl;
	cout << "\t\t\t\t\t\t---------------------" << endl;
	cout << "\t\t\t\t\t\t S  M  T  W  T  F  S" << endl;

	int dom = daysOfMonth(d.month, d.year);
	Date temp = d;
	temp.day = 1;
	int countDayOfWeek = dayOfWeek(temp);
	switch (countDayOfWeek) {
		case 0:
			cout << "\t\t\t\t\t\t";
			break;
		case 1:
			cout << "\t\t\t\t\t\t   ";
			break;
		case 2:
			cout << "\t\t\t\t\t\t      ";
			break;
		case 3:
			cout << "\t\t\t\t\t\t         ";
			break;
		case 4:
			cout << "\t\t\t\t\t\t            ";
			break;
		case 5:
			cout << "\t\t\t\t\t\t               ";
			break;
		case 6:
			cout << "\t\t\t\t\t\t                  ";
			break;
	}

	for (int i=1 ; i <= dom; i++) {
		if (temp.day == d.day && temp.month == d.month) {
			BackGroundColor(5);
			cout << right << setw(2) << i << " ";
			BackGroundColor(0);
		}
		else if (temp.day == _now->day && temp.day == _now->month) {
			BackGroundColor(1);
			cout << right << setw(2) << i << " ";
			BackGroundColor(0);
		}
		else if (checkNote(temp)) {
			BackGroundColor(12);
			cout << right << setw(2) << i << " ";
			BackGroundColor(0);
		}
		else if (countDayOfWeek == 0) {
			TextColor(12);
			cout << right << setw(2) << i << " ";
			TextColor(7);
		}
		else
			cout << right << setw(2) << i << " ";
		if (++countDayOfWeek > 6) {
			countDayOfWeek = 0;
			cout << endl << "\t\t\t\t\t\t";
		}
		temp.day++;
	}
	cout << endl << endl;
}

void Calendar::showCalendar() {
	Date temp = *_now;
	
	char c;
	while (true) {
		system("cls");
		printCalendar(temp);

		cout << endl << endl << endl;
		cout << "\t\t\t\t\t\tPress 'a' to add note\n";
		cout << "\t\t\t\t\t\tPress 's' to see note\n";
		cout << "\t\t\t\t\t\tPress 'f' to find date\n";
		cout << "\t\t\t\t\t\tPress 'n' to see next month\n";
		cout << "\t\t\t\t\t\tPress 'p' to see previous month\n";
		cout << "\t\t\t\t\t\tPress 'q' to quit\n";
		cout << "\t\t\t\t\t\t>";
		c = _getch();
		_strupr(&c);
		switch (c) {
			case -32:
				c = _getch();
				switch ((int)c) {
					case UP_ARROW:
						decreaseDay(temp,7);
						break;
					case DOWN_ARROW:
						increaseDay(temp,7);
						break;
					case LEFT_ARROW:
						decreaseDay(temp,1);
						break;
					case RIGHT_ARROW:
						increaseDay(temp,1);
						break;
				}
				break;
			case 'A':
				addNote(temp);
				break;
			case 'S':
				seeNote(temp);
				break;
			case 'F':
				Date x;
				char k;
				cout << "\n\t\t\t\t\t\tEnter the date(dd/mm/yyyy): ";
				cin >> x.day >> k >> x.month >> k >> x.year;
				findDate(x);
				break;
			case 'N':
				increaseMonth(temp);
				break;
			case 'P':
				decreaseMonth(temp);
				break;
			case 'Q':
				exit(0);
			
		}
	}
}

bool Calendar::checkNote(Date d) {
	fstream f;
	f.open(NOTE_FILE, ios::in);
	Date temp;
	string tempNote;

	while (!f.eof()) {
		f >> temp.day >> temp.month >> temp.year;
		getline(f, tempNote);
		getline(f, tempNote);
		if (temp.day == d.day && temp.month == d.month && temp.year == d.year) {
			f.close();
			return true;
		}
	}
	f.close();
	return false;
}

void Calendar::addNote(Date d) {
	fstream fin, fout;
	Date temp;
	string tempNote, note;

	seeNote(d);
	cout << "\t\t\t\t\t\tThe new note: ";
	getline(cin, note);
	
	fin.open(NOTE_FILE, ios::in);
	fout.open("temp.txt", ios::out);

	while (!fin.eof()) {
		fin >> temp.day >> temp.month >> temp.year;
		getline(fin, tempNote);
		getline(fin, tempNote);

		if (temp.day == d.day && temp.month == d.month & temp.year == d.year)
			continue;
		if (tempNote != "") {
			fout << temp.day << " " << temp.month << " " << temp.year << endl;
			fout << tempNote << endl;
		}
		
	}

	if (note != "") {
		fout << d.day << " " << d.month << " " << d.year << endl;
		fout << note << endl;
	}

	fin.close();
	fout.close();

	remove(NOTE_FILE);
	rename("temp.txt", NOTE_FILE);
}

void Calendar::seeNote(Date d) {
	system("cls");
	printCalendar(d);

	printf("\n\n\t\t\t\t\t\t%02d/%02d/%04d\n", d.day, d.month, d.year);
	if (checkNote(d)) {
		fstream f;
		f.open(NOTE_FILE, ios::in);
		Date temp;
		string tempNote;

		while (!f.eof()) {
			f >> temp.day >> temp.month >> temp.year;
			getline(f, tempNote);
			getline(f, tempNote);
			if (temp.day == d.day && temp.month == d.month && temp.year == d.year) {
				cout << "\t\t\t\t\t\tThe last note: " << tempNote << endl;
				f.close();
				getchar();
				return;
			}
		}
	}
	cout << "\t\t\t\t\t\t";
	system("pause");
}

void Calendar::findDate(Date x) {
	system("cls");
	if (x.year >= 1900 || x.year <= 2200) {
		if (x.month > 0 && x.month < 13) {
			if (x.day > 0 && x.day < daysOfMonth(x.month, x.year)) {
				char c;
				while (true) {
					system("cls");
					printCalendar(x);

					cout << endl << endl << endl;
					cout << "\t\t\t\t\t\tPress 'a' to add note\n";
					cout << "\t\t\t\t\t\tPress 's' to see note\n";
					cout << "\t\t\t\t\t\tPress 'n' to see next month\n";
					cout << "\t\t\t\t\t\tPress 'p' to see previous month\n";
					cout << "\t\t\t\t\t\tPress 'b' to back\n";
					cout << "\t\t\t\t\t\t>";
					c = _getch();
					_strupr(&c);
					switch (c) {
					case -32:
						c = _getch();
						switch ((int)c) {
						case UP_ARROW:
							decreaseDay(x, 7);
							break;
						case DOWN_ARROW:
							increaseDay(x, 7);
							break;
						case LEFT_ARROW:
							decreaseDay(x, 1);
							break;
						case RIGHT_ARROW:
							increaseDay(x, 1);
							break;
						}
						break;
					case 'A':
						addNote(x);
						break;
					case 'S':
						seeNote(x);
						break;
					case 'N':
						increaseMonth(x);
						break;
					case 'P':
						decreaseMonth(x);
						break;
					case 'B':
						return;
					}
				}


			}
		}
	}
	cout << "Cant find";
}