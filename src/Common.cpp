#include "Common.h"

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

bool Date::tryParse(const std::string& str, Date& outDate) {
    int d = 0, m = 0, y = 0;
    char sep1 = 0, sep2 = 0;
    std::istringstream iss(str);
    if ((iss >> d >> sep1 >> m >> sep2 >> y) && (sep1 == '/' || sep1 == '-') && (sep2 == '/' || sep2 == '-')) {
        Date tmp(d, m, y);
        if (tmp.isValid() && y >= 1900 && y <= 2100) {
            outDate = tmp;
            return true;
        }
    }
    return false;
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

int Date::toDays() const {
    int days = 0;
    for (int y = 1900; y < year; ++y) {
        days += isLeapYear(y) ? 366 : 365;
    }
    for (int m = 1; m < month; ++m) {
        days += daysInMonth(m, year);
    }
    days += day;
    return days;
}

int Date::daysBetween(const Date& other) const {
    return other.toDays() - this->toDays();
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

// =========================================================================
// 2. CÀI ĐẶT FACULTY (ĐỒNG BỘ 10 KHOA)
// =========================================================================

const std::vector<std::string>& Faculty::getList() {
    static const std::vector<std::string> faculties = {
        "Cong nghe Thong tin",
        "Dien - Dien tu",
        "Co khi",
        "Xay dung",
        "Kien truc",
        "Co khi Giao thong",
        "Hoa",
        "Moi truong",
        "Quan ly Du an",
        "Nhiet - Dien lanh"
    };
    return faculties;
}

void Faculty::printMenu() {
    std::cout << "\n╔══════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                 DANH MỤC 10 KHOA CHUẨN ĐẠI HỌC BÁCH KHOA                 ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
    const auto& list = getList();
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << "║   [" << std::setw(2) << (i + 1) << "] " << std::left << std::setw(66) << list[i] << "║\n";
    }
    std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
}

std::string Faculty::getByIndex(int index) {
    const auto& list = getList();
    if (index >= 1 && index <= static_cast<int>(list.size())) {
        return list[index - 1];
    }
    return "Khac";
}

bool Faculty::isValidIndex(int index) {
    return index >= 1 && index <= static_cast<int>(getList().size());
}

std::string Faculty::normalizeFaculty(const std::string& input) {
    if (input == "CNTT" || input == "cntt") return "Cong nghe Thong tin";
    if (input == "Dien" || input == "Dien - Dien tu") return "Dien - Dien tu";
    if (input == "DTVT") return "Dien tu - Vien thong";
    if (input == "Kien truc") return "Kien truc";
    for (const auto& f : getList()) {
        if (f == input) return f;
    }
    return input;
}

// =========================================================================
// 3. CÀI ĐẶT CLASSVALIDATOR (TÍNH KHÓA & RÀ SOÁT LỚP)
// =========================================================================

std::string ClassValidator::extractCohort(const std::string& className) {
    if (className.length() >= 2 && std::isdigit(className[0]) && std::isdigit(className[1])) {
        return "K" + className.substr(0, 2);
    }
    return "K22";
}

bool ClassValidator::isValidClassName(const std::string& className, std::string& errorReason) {
    if (className.length() < 4 || className.length() > 15) {
        errorReason = "Độ dài tên lớp phải từ 4 đến 15 ký tự (ví dụ: 22T_DT1, 22KT, 23T_DT1).";
        return false;
    }
    if (!std::isdigit(className[0]) || !std::isdigit(className[1])) {
        errorReason = "2 ký tự đầu tiên BẮT BUỘC là 2 chữ số năm khóa học (ví dụ: '22' trong 22T_DT1, '23' trong 23T_DT1).";
        return false;
    }
    int yr = std::stoi(className.substr(0, 2));
    if (yr < 18 || yr > 26) {
        errorReason = "Năm khóa học phải từ 18 (K18) đến 26 (K26).";
        return false;
    }
    for (size_t i = 2; i < className.length(); ++i) {
        char ch = className[i];
        if (!std::isalnum(ch) && ch != '_') {
            errorReason = "Tên lớp chỉ được chứa chữ cái, chữ số và dấu gạch dưới '_' (không dùng dấu cách hay ký tự lạ).";
            return false;
        }
    }
    return true;
}

bool ClassValidator::isValidClassName(const std::string& className) {
    std::string reason;
    return isValidClassName(className, reason);
}

std::vector<std::string> ClassValidator::loadClasses(const std::string& filePath) {
    std::vector<std::string> classes;
    std::ifstream file(filePath);
    if (file.is_open()) {
        try {
            nlohmann::json jArr;
            file >> jArr;
            for (const auto& item : jArr) {
                classes.push_back(item.get<std::string>());
            }
        } catch (...) {}
    }
    return classes;
}

bool ClassValidator::isClassRegistered(const std::string& className, const std::string& filePath) {
    auto list = loadClasses(filePath);
    return std::find(list.begin(), list.end(), className) != list.end();
}

void ClassValidator::registerClass(const std::string& className, const std::string& filePath) {
    auto list = loadClasses(filePath);
    if (std::find(list.begin(), list.end(), className) == list.end()) {
        list.push_back(className);
        std::sort(list.begin(), list.end());
        std::ofstream file(filePath);
        if (file.is_open()) {
            nlohmann::json jArr = list;
            file << jArr.dump(2);
        }
    }
}
