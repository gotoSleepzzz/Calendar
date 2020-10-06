#include"Date.h"

bool isLeapYear(int year) {
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return true;
	return false;
}

int dayOfWeek(const Date& d) {
	int yearCode = d.year % 100;
	yearCode = (yearCode + (yearCode / 4)) % 7;

	int monthCode;
	switch (d.month) {
	case 1: case 10:
		monthCode = 0;
		break;
	case 5:
		monthCode = 1;
		break;
	case 8:
		monthCode = 2;
		break;
	case 2: case 3: case 11:
		monthCode = 3;
		break;
	case 6:
		monthCode = 4;
		break;
	case 9: case 12:
		monthCode = 5;
		break;
	case 4: case 7:
		monthCode = 6;
		break;
	}

	int centuryCode;
	switch (d.year / 100) {
	case 18:
		centuryCode = 2;
		break;
	case 19:
		centuryCode = 0;
		break;
	case 20:
		centuryCode = 6;
		break;
	case 21:
		centuryCode = 4;
		break;
	}

	int leapCode(0);
	if (isLeapYear(d.year) && (d.month == 1 || d.month == 2))
		leapCode = 1;

	return (yearCode + monthCode + centuryCode + d.day - leapCode) % 7;
}

int daysOfMonth(int month,int year) {
	switch (month) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
		return 31;
	case 4: case 6: case 9: case 11:
		return 30;
	case 2:
		if (isLeapYear(year))
			return 29;
		return 28;
	}
}

void increaseDay(Date& d,int n) {
	int c = daysOfMonth(d.month,d.year);
	d.day += n;
	if (d.day > c) {
		d.day = d.day - c;
		increaseMonth(d);
	}
}

void decreaseDay(Date& d,int n) {
	if (n >= d.day) {
		decreaseMonth(d);
		d.day = daysOfMonth(d.month,d.year) - n + d.day;
	}
	else
		d.day -= n;

}

void increaseMonth(Date& d) {
	if (d.month == 12) {
		d.month = 1;
		d.year++;
	}
	else
		d.month++;
}

void decreaseMonth(Date& d) {
	if (d.month == 1) {
		d.month = 12;
		d.year--;
	}
	else
		d.month--;
}