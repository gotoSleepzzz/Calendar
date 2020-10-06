#ifndef _DATE_H_
#define _DATE_H_

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<time.h>

using namespace std;

const string MON[12] = { "January","February","March","April", "May", "June","July", "August","September","October","November", "December" };

struct Date {
	int day;
	int month;
	int year;
};

bool isLeapYear(int year);

int dayOfWeek(const Date& d);

int daysOfMonth(int month, int year);

void increaseDay(Date& d, int n);

void decreaseDay(Date& d, int n);

void increaseMonth(Date& d);

void decreaseMonth(Date& d);


#endif
