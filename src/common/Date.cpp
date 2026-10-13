#include "common/Date.h"
#include <iomanip>
#include <sstream>

bool Date::isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::daysInMonth(int m, int y) {
    switch (m) {
        case 2:
            return isLeapYear(y) ? 29 : 28;
        case 4: case 6: case 9: case 11:
            return 30;
        default:
            return 31;
    }
}

Date::Date() : day(1), month(1), year(2026) {}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    if (!isValid()) {
        day = 1;
        month = 1;
        year = 2026;
    }
}

int Date::getDay() const { return day; }
int Date::getMonth() const { return month; }
int Date::getYear() const { return year; }

void Date::setDay(int d) { day = d; }
void Date::setMonth(int m) { month = m; }
void Date::setYear(int y) { year = y; }

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > daysInMonth(month, year)) return false;
    return true;
}

std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << day << "/"
        << std::setw(2) << std::setfill('0') << month << "/"
        << year;
    return oss.str();
}

Date Date::fromString(const std::string& str) {
    int d = 1, m = 1, y = 2026;
    char sep1, sep2;
    std::istringstream iss(str);
    if (iss >> d >> sep1 >> m >> sep2 >> y) {
        return Date(d, m, y);
    }
    return Date(1, 1, 2026);
}

bool Date::isBefore(const Date& other) const {
    return *this < other;
}

Date Date::addMonths(int months) const {
    int totalMonths = (year * 12) + (month - 1) + months;
    int newYear = totalMonths / 12;
    int newMonth = (totalMonths % 12) + 1;
    int maxDays = daysInMonth(newMonth, newYear);
    int newDay = (day > maxDays) ? maxDays : day;
    return Date(newDay, newMonth, newYear);
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}

bool Date::operator>(const Date& other) const {
    return !(*this <= other);
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Date& dt) {
    os << dt.toString();
    return os;
}
