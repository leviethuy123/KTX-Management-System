#include "Common.h"
#include "TablePrinter.h"

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

Date Date::now() {
    std::time_t t = std::time(nullptr);
#if defined(_MSC_VER)
    std::tm localTime;
    localtime_s(&localTime, &t);
    return Date(localTime.tm_mday, localTime.tm_mon + 1, localTime.tm_year + 1900);
#else
    std::tm* localTime = std::localtime(&t);
    if (!localTime) {
        return Date(1, 1, 2026);
    }
    return Date(localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
#endif
}

Date Date::today() {
    return now();
}

Date Date::getCurrentDate() {
    return now();
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
// 2. CÀI ĐẶT FACULTY (ĐỒNG BỘ 8 KHOA CHUẨN ĐHBK)
// =========================================================================

const std::vector<std::string>& Faculty::getList() {
    static const std::vector<std::string> faculties = {
        "Cơ khí Giao thông và Năng lượng",
        "Công nghệ thông tin",
        "Điện",
        "Điện tử và Trí tuệ nhân tạo",
        "Hóa, Môi trường và KHSS",
        "Xây dựng",
        "Quản lí dự án và Công nghiệp",
        "Kiến trúc"
    };
    return faculties;
}

void Faculty::printMenu() {
    std::cout << "\n╔══════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                 DANH MỤC 8 KHOA CHUẨN ĐẠI HỌC BÁCH KHOA                  ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════╣\n";
    const auto& list = getList();
    for (size_t i = 0; i < list.size(); ++i) {
        std::string line = "[" + std::to_string(i + 1) + "] " + list[i];
        std::cout << "║   " << StringUtils::padRight(line, 69) << "║\n";
    }
    std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
}

std::string Faculty::getByIndex(int index) {
    const auto& list = getList();
    if (index >= 1 && index <= static_cast<int>(list.size())) {
        return list[index - 1];
    }
    return "Khác";
}

bool Faculty::isValidIndex(int index) {
    return index >= 1 && index <= static_cast<int>(getList().size());
}

std::string Faculty::normalizeFaculty(const std::string& input) {
    if (input == "CNTT" || input == "cntt" || input == "Cong nghe Thong tin") return "Công nghệ thông tin";
    if (input == "Dien" || input == "Dien - Dien tu") return "Điện";
    if (input == "DTVT" || input == "Dien tu va Tri tue nhan tao") return "Điện tử và Trí tuệ nhân tạo";
    if (input == "Kien truc") return "Kiến trúc";
    if (input == "Xay dung") return "Xây dựng";
    if (input == "Hoa" || input == "Moi truong" || input == "Hoa, Moi truong va KHSS") return "Hóa, Môi trường và KHSS";
    if (input == "Quan ly Du an" || input == "Quan ly Du an va Cong nghiep") return "Quản lí dự án và Công nghiệp";
    if (input == "Co khi" || input == "Co khi Giao thong" || input == "Co khi Giao thong va Nang luong" || input == "Nhiet - Dien lanh") return "Cơ khí Giao thông và Năng lượng";
    for (const auto& f : getList()) {
        if (f == input) return f;
    }
    return input;
}

// =========================================================================
// 3. CÀI ĐẶT CLASSVALIDATOR (TÍNH KHÓA, TRÍCH XUẤT KHOA & BẪY LỖI LỚP)
// =========================================================================

std::string ClassValidator::extractCohort(const std::string& className) {
    if (className.length() >= 2 && std::isdigit(className[0]) && std::isdigit(className[1])) {
        return "K" + className.substr(0, 2);
    }
    return "K22";
}

std::string ClassValidator::extractFaculty(const std::string& className) {
    if (className.length() < 3) return "";

    // Bỏ qua 2 chữ số năm khóa học ở đầu (ví dụ: "22T_DT1" -> "T_DT1", "22KT" -> "KT")
    std::string code = className.substr(2);
    for (char& c : code) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }

    // 1. Tiền tố 5 ký tự: KTOTO -> Cơ khí Giao thông và Năng lượng
    if (code.rfind("KTOTO", 0) == 0) {
        return "Cơ khí Giao thông và Năng lượng";
    }

    // 2. Tiền tố 4 ký tự: KTTT -> Cơ khí Giao thông và Năng lượng
    if (code.rfind("KTTT", 0) == 0) {
        return "Cơ khí Giao thông và Năng lượng";
    }

    // 3. Tiền tố 3 ký tự: TDH -> Điện
    if (code.rfind("TDH", 0) == 0) {
        return "Điện";
    }

    // 4. Tiền tố 2 ký tự:
    // Riêng KT: KTOTO và KTTT đã được lọc ở trên, nên KT còn lại là Khoa Kiến trúc
    if (code.rfind("KT", 0) == 0) {
        return "Kiến trúc";
    }
    if (code.rfind("DT", 0) == 0 || code.rfind("PF", 0) == 0 ||
        code.rfind("EC", 0) == 0 || code.rfind("ES", 0) == 0) {
        return "Điện tử và Trí tuệ nhân tạo";
    }
    if (code.rfind("SH", 0) == 0) {
        return "Hóa, Môi trường và KHSS";
    }
    if (code.rfind("CS", 0) == 0 || code.rfind("VL", 0) == 0 || code.rfind("BI", 0) == 0) {
        return "Xây dựng";
    }
    if (code.rfind("QL", 0) == 0 || code.rfind("KX", 0) == 0) {
        return "Quản lí dự án và Công nghiệp";
    }

    // 5. Tiền tố 1 ký tự:
    if (code.rfind("C", 0) == 0 || code.rfind("N", 0) == 0) {
        return "Cơ khí Giao thông và Năng lượng";
    }
    if (code.rfind("T", 0) == 0) {
        return "Công nghệ thông tin";
    }
    if (code.rfind("D", 0) == 0) {
        return "Điện";
    }
    if (code.rfind("H", 0) == 0) {
        return "Hóa, Môi trường và KHSS";
    }
    if (code.rfind("X", 0) == 0) {
        return "Xây dựng";
    }

    return "";
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
    std::string fac = extractFaculty(className);
    if (fac.empty()) {
        errorReason = "Mã ngành trong tên lớp '" + className + "' không thuộc bất kỳ Khoa nào trong 8 Khoa của trường!\n"
                      "  (Gợi ý mã ngành hợp lệ: T (CNTT), D/TDH (Điện), DT/PF/EC/ES (Điện tử & AI), C/KTOTO/KTTT/N (CKGT & NL),\n"
                      "   H/SH (Hóa & MT), X/CS/VL/BI (Xây dựng), QL/KX (QLDA & CN), KT (Kiến trúc)).";
        return false;
    }
    return true;
}

bool ClassValidator::isValidClassName(const std::string& className) {
    std::string reason;
    return isValidClassName(className, reason);
}
