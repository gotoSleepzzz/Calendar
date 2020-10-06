#ifndef _CALENDAR_H_
#define _CALENDAR_H_

#define _CRT_SECURE_NO_WARNINGS
#include<iomanip>
#include<conio.h>
#include<fstream>
#include<string>
#include<Windows.h>
#include"Date.h"

#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define NOTE_FILE "note.txt"

using namespace std;

class Calendar {
	private:
		static Calendar* _instance;
		static Date* _now;
		void printCalendar(const Date& d);
		Calendar();
	public:
		static Calendar* getObject();
		Date* getTimeNow();
		void showCalendar();
		bool checkNote(Date d);
		void addNote(Date d);
		void seeNote(Date d);
		void findDate(Date x);
};

void BackGroundColor(WORD color);

void TextColor(WORD color);

#endif