#include "Common.h"
#include "Human.h"
#include "Room.h"
#include "Rent.h"
#include "Account.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <utility>
#include <stdexcept>

// =========================================================================
// 0. CÀI ĐẶT CÁC PHƯƠNG THỨC CỦA CLASS VECTOR (NỘI DUNG THÂN HÀM)
// =========================================================================


template <typename T>
Vector<T>::Vector(const Vector& other) {
    reserve(other.n);
    for (std::size_t i = 0; i < other.n; ++i) push_back(other.arr[i]);
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept {
    swap(other);
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> other) noexcept {
    swap(other);
    return *this;
}

template <typename T>
Vector<T>::~Vector() {
    delete[] arr;
}

template <typename T>
std::size_t Vector<T>::size() const {
    return n;
}

template <typename T>
std::size_t Vector<T>::capacity() const {
    return cap;
}

template <typename T>
bool Vector<T>::empty() const {
    return n == 0;
}

template <typename T>
T& Vector<T>::operator[](std::size_t i) {
    return arr[i];
}

template <typename T>
const T& Vector<T>::operator[](std::size_t i) const {
    return arr[i];
}

template <typename T>
T& Vector<T>::at(std::size_t i) {
    if (i >= n) throw std::out_of_range("Vector::at");
    return arr[i];
}

template <typename T>
void Vector<T>::reserve(std::size_t newCap) {
    if (newCap <= cap) return;
    T* p = new T[newCap];
    for (std::size_t i = 0; i < n; ++i) p[i] = std::move(arr[i]);
    delete[] arr;
    arr = p;
    cap = newCap;
}

template <typename T>
void Vector<T>::push_back(T x) {
    if (n == cap) reserve(cap == 0 ? 4 : cap * 2);
    arr[n++] = std::move(x);
}

template <typename T>
void Vector<T>::pop_back() {
    if (n > 0) --n;
}

template <typename T>
void Vector<T>::clear() {
    n = 0;
}

template <typename T>
void Vector<T>::erase(std::size_t i) {
    if (i >= n) return;
    for (; i + 1 < n; ++i) arr[i] = std::move(arr[i + 1]);
    --n;
}

template <typename T>
T* Vector<T>::erase(T* it) {
    if (it < arr || it >= arr + n) return it;
    std::size_t idx = static_cast<std::size_t>(it - arr);
    erase(idx);
    return arr + idx;
}

template <typename T>
void Vector<T>::swap(Vector<T>& other) noexcept {
    std::swap(arr, other.arr);
    std::swap(n, other.n);
    std::swap(cap, other.cap);
}

template <typename T>
T* Vector<T>::begin() {
    return arr;
}

template <typename T>
T* Vector<T>::end() {
    return arr + n;
}

template <typename T>
const T* Vector<T>::begin() const {
    return arr;
}

template <typename T>
const T* Vector<T>::end() const {
    return arr + n;
}

template <typename T>
const T* Vector<T>::cbegin() const {
    return arr;
}

template <typename T>
const T* Vector<T>::cend() const {
    return arr + n;
}

// Explicit template instantiations cho toàn bộ 8 kiểu dữ liệu của hệ thống
template class Vector<std::string>;
template class Vector<Student*>;
template class Vector<Room*>;
template class Vector<Rent*>;
template class Vector<Staff*>;
template class Vector<Account*>;
template class Vector<BrokenDevice>;
template class Vector<RoomRevenueRow>;

// =========================================================================
// 1. CÀI ĐẶT LỚP XỬ LÝ NGÀY THÁNG (DATE)
// =========================================================================

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
// =========================================================================
// 2. CÀI ĐẶT FACULTY (ĐỒNG BỘ 8 KHOA CHUẨN ĐHBK)
// =========================================================================

const Vector<std::string>& Faculty::getList() {
    static Vector<std::string> faculties;
    if (faculties.empty()) {
        faculties.reserve(8);
        faculties.push_back("Cơ khí Giao thông và Năng lượng");
        faculties.push_back("Công nghệ thông tin");
        faculties.push_back("Điện");
        faculties.push_back("Điện tử và Trí tuệ nhân tạo");
        faculties.push_back("Hóa, Môi trường và KHSS");
        faculties.push_back("Xây dựng");
        faculties.push_back("Quản lí dự án và Công nghiệp");
        faculties.push_back("Kiến trúc");
    }
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

// =========================================================================
// 4. TIỆN ÍCH TÍNH TOÁN ĐỘ RỘNG VISUAL VÀ OFFSET UTF-8 TIẾNG VIỆT
// =========================================================================

namespace StringUtils {
    int utf8Width(const std::string& str) {
        int w = 0;
        for (size_t i = 0; i < str.length(); ++i) {
            unsigned char c = static_cast<unsigned char>(str[i]);
            // Mỗi ký tự UTF-8 chỉ có 1 byte mở đầu (khác byte tiếp nối 10xxxxxx)
            if ((c & 0xC0) != 0x80) {
                w++;
            }
        }
        return w;
    }

    int utf8Offset(const std::string& str) {
        return static_cast<int>(str.length()) - utf8Width(str);
    }

    std::string padRight(const std::string& str, int visualWidth) {
        int w = utf8Width(str);
        if (w >= visualWidth) return str;
        return str + std::string(visualWidth - w, ' ');
    }

    std::string padLeft(const std::string& str, int visualWidth) {
        int w = utf8Width(str);
        if (w >= visualWidth) return str;
        return std::string(visualWidth - w, ' ') + str;
    }

    std::string padCenter(const std::string& str, int visualWidth) {
        int w = utf8Width(str);
        if (w >= visualWidth) return str;
        int left = (visualWidth - w) / 2;
        int right = visualWidth - w - left;
        return std::string(left, ' ') + str + std::string(right, ' ');
    }

    std::string repeat(const std::string& pattern, int count) {
        std::string res;
        for (int i = 0; i < count; ++i) res += pattern;
        return res;
    }

    std::string formatMoney(double amount) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << amount << " VND";
        return oss.str();
    }
}

using StringUtils::formatMoney;

// =========================================================================
// 5. CÁC HÀM IN BẢNG DỮ LIỆU ĐƯỢC CANH CHỈNH OFFSET CHUẨN XÁC
// =========================================================================

void TablePrinter::printStudents(const Vector<Student*>& list) {
    if (list.empty()) {
        std::cout << "(Không có sinh viên nào)\n";
        return;
    }
    std::cout << "╔════════╦══════════════════════╦════════╦════════╦═════════════╦═══════╦════════════════════════╦════════════╦═════════════════════════╦════════╗\n";
    std::cout << "║ " << StringUtils::padRight("MÃ SV", 6)
              << " ║ " << StringUtils::padRight("HỌ VÀ TÊN", 20)
              << " ║ " << StringUtils::padRight("GIỚI", 6)
              << " ║ " << StringUtils::padRight("NĂM", 6)
              << " ║ " << StringUtils::padRight("LỚP", 11)
              << " ║ " << StringUtils::padRight("KHÓA", 5)
              << " ║ " << StringUtils::padRight("KHOA", 22)
              << " ║ " << StringUtils::padRight("SĐT", 10)
              << " ║ " << StringUtils::padRight("EMAIL", 23)
              << " ║ " << StringUtils::padRight("PHÒNG", 6) << " ║\n";
    std::cout << "╠════════╬══════════════════════╬════════╬════════╬═════════════╬═══════╬════════════════════════╬════════════╬═════════════════════════╬════════╣\n";
    for (const Student* s : list) {
        if (s == nullptr) continue;
        std::string genderStr = (s->getGender() == Gender::MALE) ? "Nam" : "Nu";
        std::string roomStr = (s->getRent() != nullptr && s->getRent()->isActive() && s->getRent()->getRoom() != nullptr)
                              ? s->getRent()->getRoom()->getId() : "(Chưa)";
        std::string nameSub = s->getFullName();
        if (StringUtils::utf8Width(nameSub) > 20) nameSub = nameSub.substr(0, 17) + "...";
        std::string classSub = s->getClass();
        if (StringUtils::utf8Width(classSub) > 11) classSub = classSub.substr(0, 11);
        std::string facSub = s->getFaculty();
        if (StringUtils::utf8Width(facSub) > 22) facSub = facSub.substr(0, 19) + "...";
        std::string emSub = s->getEmail();
        if (StringUtils::utf8Width(emSub) > 23) emSub = emSub.substr(0, 20) + "...";

        std::cout << "║ " << StringUtils::padRight(s->getId(), 6)
                  << " ║ " << StringUtils::padRight(nameSub, 20)
                  << " ║ " << StringUtils::padRight(genderStr, 6)
                  << " ║ " << StringUtils::padRight(s->getBirthYear(), 6)
                  << " ║ " << StringUtils::padRight(classSub, 11)
                  << " ║ " << StringUtils::padRight(s->getCohort(), 5)
                  << " ║ " << StringUtils::padRight(facSub, 22)
                  << " ║ " << StringUtils::padRight(s->getPhone(), 10)
                  << " ║ " << StringUtils::padRight(emSub, 23)
                  << " ║ " << StringUtils::padRight(roomStr, 6)
                  << " ║\n";
    }
    std::cout << "╚════════╩══════════════════════╩════════╩════════╩═════════════╩═══════╩════════════════════════╩════════════╩═════════════════════════╩════════╝\n";
    std::cout << ">> TỔNG SỐ SINH VIÊN: " << list.size() << " sinh viên\n";
}

void TablePrinter::printRooms(const Vector<Room*>& list) {
    if (list.empty()) {
        std::cout << "(Không có phòng nào)\n";
        return;
    }
    std::cout << "╔════════╦════════════╦══════╦══════════╦══════════╦════════╦═══════════╦═══════════════╦════════════╦════════════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("PHÒNG", 6)
              << " ║ " << StringUtils::padRight("TÒA NHÀ", 10)
              << " ║ " << StringUtils::padRight("TẦNG", 4)
              << " ║ " << StringUtils::padRight("LOẠI", 8)
              << " ║ " << StringUtils::padRight("SỨC CHỨA", 8)
              << " ║ " << StringUtils::padRight("ĐANG Ở", 6)
              << " ║ " << StringUtils::padRight("CÒN TRỐNG", 9)
              << " ║ " << StringUtils::padRight("GIÁ PHÒNG", 13)
              << " ║ " << StringUtils::padRight("TRẠNG THÁI", 10)
              << " ║ " << StringUtils::padRight("THIẾT BỊ HỎNG", 18) << " ║\n";
    std::cout << "╠════════╬════════════╬══════╬══════════╬══════════╬════════╬═══════════╬═══════════════╬════════════╬════════════════════╣\n";
    for (const Room* r : list) {
        if (r == nullptr) continue;
        int occupants = static_cast<int>(r->getActiveRent().size());
        int freeSlots = r->getMaxStudents() - occupants;
        int brokenCount = r->countUnfixedDevices();
        std::string brokenStr = (brokenCount > 0) ? ("Có " + std::to_string(brokenCount) + " thiết bị") : "Hoạt động tốt";

        std::cout << "║ " << StringUtils::padRight(r->getId(), 6)
                  << " ║ " << StringUtils::padRight(r->getBuilding(), 10)
                  << " ║ " << StringUtils::padRight(std::to_string(r->getFloor()), 4)
                  << " ║ " << StringUtils::padRight(r->getRoomType(), 8)
                  << " ║ " << StringUtils::padRight(std::to_string(r->getMaxStudents()), 8)
                  << " ║ " << StringUtils::padRight(std::to_string(occupants), 6)
                  << " ║ " << StringUtils::padRight(std::to_string(freeSlots), 9)
                  << " ║ " << StringUtils::padRight(formatMoney(r->getBasePrice()), 13)
                  << " ║ " << StringUtils::padRight(EnumHelper::roomStatusToString(r->getStatus()), 10)
                  << " ║ " << StringUtils::padRight(brokenStr, 18)
                  << " ║\n";
    }
    std::cout << "╚════════╩════════════╩══════╩══════════╩══════════╩════════╩═══════════╩═══════════════╩════════════╩════════════════════╝\n";
    std::cout << ">> TỔNG SỐ PHÒNG: " << list.size() << " phòng\n";
}

void TablePrinter::printRents(const Vector<Rent*>& list) {
    if (list.empty()) {
        std::cout << "(Không có hợp đồng nào)\n";
        return;
    }
    std::cout << "╔════════╦════════╦══════════════════════╦════════╦════════════╦════════════╦══════════╦═══════════════╦════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("MÃ HĐ", 6)
              << " ║ " << StringUtils::padRight("MÃ SV", 6)
              << " ║ " << StringUtils::padRight("TÊN SINH VIÊN", 20)
              << " ║ " << StringUtils::padRight("PHÒNG", 6)
              << " ║ " << StringUtils::padRight("NGÀY BĐ", 10)
              << " ║ " << StringUtils::padRight("NGÀY KT", 10)
              << " ║ " << StringUtils::padRight("THỜI HẠN", 8)
              << " ║ " << StringUtils::padRight("GIÁ THUÊ/THÁNG", 11)
              << " ║ " << StringUtils::padRight("TRẠNG THÁI", 10) << " ║\n";
    std::cout << "╠════════╬════════╬══════════════════════╬════════╬════════════╬════════════╬══════════╬═══════════════╬════════════╣\n";
    for (const Rent* r : list) {
        if (r == nullptr) continue;
        std::string sId = r->getTenant() ? r->getTenant()->getId() : "N/A";
        std::string sName = r->getTenant() ? r->getTenant()->getFullName() : "N/A";
        if (StringUtils::utf8Width(sName) > 20) sName = sName.substr(0, 17) + "...";
        std::string rId = r->getRoom() ? r->getRoom()->getId() : "N/A";

        std::cout << "║ " << StringUtils::padRight(r->getId(), 6)
                  << " ║ " << StringUtils::padRight(sId, 6)
                  << " ║ " << StringUtils::padRight(sName, 20)
                  << " ║ " << StringUtils::padRight(rId, 6)
                  << " ║ " << StringUtils::padRight(r->getStartDate().toString(), 10)
                  << " ║ " << StringUtils::padRight(r->getActualEndDate().toString(), 10)
                  << " ║ " << StringUtils::padRight(EnumHelper::periodToString(r->getRentalPeriod()), 8)
                  << " ║ " << StringUtils::padRight(formatMoney(r->getRentFee()), 13)
                  << " ║ " << StringUtils::padRight(EnumHelper::rentStatusToString(r->getStatus()), 10)
                  << " ║\n";
    }
    std::cout << "╚════════╩════════╩══════════════════════╩════════╩════════════╩════════════╩══════════╩═══════════════╩════════════╝\n";
    std::cout << ">> TỔNG SỐ HỢP ĐỒNG: " << list.size() << " hợp đồng\n";
}

void TablePrinter::printStaffs(const Vector<Staff*>& list) {
    if (list.empty()) {
        std::cout << "(Không có cán bộ nào)\n";
        return;
    }
    std::cout << "╔════════╦══════════════════════╦════════╦════════════════════════╦═════════════════════════╦════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("MÃ CB", 6)
              << " ║ " << StringUtils::padRight("HỌ VÀ TÊN", 20)
              << " ║ " << StringUtils::padRight("GIỚI", 6)
              << " ║ " << StringUtils::padRight("PHÒNG BAN", 22)
              << " ║ " << StringUtils::padRight("CHỨC VỤ", 23)
              << " ║ " << StringUtils::padRight("SĐT", 10) << " ║\n";
    std::cout << "╠════════╬══════════════════════╬════════╬════════════════════════╬═════════════════════════╬════════════╣\n";
    for (const Staff* st : list) {
        if (st == nullptr) continue;
        std::string genderStr = (st->getGender() == Gender::MALE) ? "Nam" : "Nu";
        std::string nameSub = st->getFullName();
        if (StringUtils::utf8Width(nameSub) > 20) nameSub = nameSub.substr(0, 17) + "...";
        std::string deptSub = st->getDepartment();
        if (StringUtils::utf8Width(deptSub) > 22) deptSub = deptSub.substr(0, 19) + "...";
        std::string posSub = st->getPosition();
        if (StringUtils::utf8Width(posSub) > 23) posSub = posSub.substr(0, 20) + "...";

        std::cout << "║ " << StringUtils::padRight(st->getId(), 6)
                  << " ║ " << StringUtils::padRight(nameSub, 20)
                  << " ║ " << StringUtils::padRight(genderStr, 6)
                  << " ║ " << StringUtils::padRight(deptSub, 22)
                  << " ║ " << StringUtils::padRight(posSub, 23)
                  << " ║ " << StringUtils::padRight(st->getPhone(), 10)
                  << " ║\n";
    }
    std::cout << "╚════════╩══════════════════════╩════════╩════════════════════════╩═════════════════════════╩════════════╝\n";
    std::cout << ">> TỔNG SỐ CÁN BỘ: " << list.size() << " người\n";
}

void TablePrinter::printAccounts(const Vector<Account*>& list) {
    if (list.empty()) {
        std::cout << "(Không có tài khoản nào)\n";
        return;
    }
    std::cout << "╔══════════╦══════════════════════╦════════════╦════════════╦════════════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("MÃ TK", 8)
              << " ║ " << StringUtils::padRight("TÊN ĐĂNG NHẬP", 20)
              << " ║ " << StringUtils::padRight("VAI TRÒ", 10)
              << " ║ " << StringUtils::padRight("MÃ LIÊN KẾT", 10)
              << " ║ " << StringUtils::padRight("TRẠNG THÁI", 18) << " ║\n";
    std::cout << "╠══════════╬══════════════════════╬════════════╬════════════╬════════════════════╣\n";
    for (const Account* a : list) {
        if (a == nullptr) continue;
        std::string stStr = a->getStatus() ? "KÍCH HOẠT" : "BỊ KHÓA";

        std::cout << "║ " << StringUtils::padRight(a->getId(), 8)
                  << " ║ " << StringUtils::padRight(a->getUsername(), 20)
                  << " ║ " << StringUtils::padRight(EnumHelper::roleToString(a->getRole()), 10)
                  << " ║ " << StringUtils::padRight(a->getOwnerId(), 10)
                  << " ║ " << StringUtils::padRight(stStr, 18)
                  << " ║\n";
    }
    std::cout << "╚══════════╩══════════════════════╩════════════╩════════════╩════════════════════╝\n";
    std::cout << ">> TỔNG SỐ TÀI KHOẢN: " << list.size() << " tài khoản\n";
}

void TablePrinter::printRoomAllocationMap(const Vector<Room*>& list) {
    if (list.empty()) {
        std::cout << "(Không có phòng nào trong KTX)\n";
        return;
    }
    std::cout << "╔════════╦══════════╦════════╦════════╦═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("PHÒNG", 6)
              << " ║ " << StringUtils::padRight("TÒA NHÀ", 8)
              << " ║ " << StringUtils::padRight("SỨC CHỨA", 6)
              << " ║ " << StringUtils::padRight("ĐANG Ở", 6)
              << " ║ " << StringUtils::padRight("DANH SÁCH SINH VIÊN ĐANG LƯU TRÚ", 59) << " ║\n";
    std::cout << "╠════════╬══════════╬════════╬════════╬═════════════════════════════════════════════════════════════╣\n";
    for (const Room* r : list) {
        if (r == nullptr) continue;
        int occupants = static_cast<int>(r->getActiveRent().size());
        std::string stdStr = "";
        for (size_t i = 0; i < r->getActiveRent().size(); ++i) {
            const Rent* rent = r->getActiveRent()[i];
            if (rent != nullptr && rent->getTenant() != nullptr) {
                if (!stdStr.empty()) stdStr += ", ";
                stdStr += rent->getTenant()->getFullName() + " (" + rent->getTenant()->getId() + ")";
            }
        }
        if (stdStr.empty()) {
            stdStr = "(Phòng trống)";
        }
        if (StringUtils::utf8Width(stdStr) > 59) {
            stdStr = stdStr.substr(0, 56) + "...";
        }

        std::cout << "║ " << StringUtils::padRight(r->getId(), 6)
                  << " ║ " << StringUtils::padRight(r->getBuilding(), 8)
                  << " ║ " << StringUtils::padRight(std::to_string(r->getMaxStudents()), 6)
                  << " ║ " << StringUtils::padRight(std::to_string(occupants), 6)
                  << " ║ " << StringUtils::padRight(stdStr, 59)
                  << " ║\n";
    }
    std::cout << "╚════════╩══════════╩════════╩════════╩═════════════════════════════════════════════════════════════╝\n";
}

void TablePrinter::printBrokenDevices(const std::string& roomId, const Vector<BrokenDevice>& list) {
    if (list.empty()) {
        std::cout << "  (Tất cả thiết bị phòng " << roomId << " đều hoạt động tốt, không có thiết bị hỏng)\n";
        return;
    }
    std::cout << "  ╔══════════╦══════════════════════════╦════════════════════════════════════════╦════════════════╗\n";
    std::cout << "  ║ " << StringUtils::padRight("MÃ TB", 8)
              << " ║ " << StringUtils::padRight("TÊN THIẾT BỊ", 24)
              << " ║ " << StringUtils::padRight("MÔ TẢ HƯ HỎNG", 38)
              << " ║ " << StringUtils::padRight("NGÀY BÁO", 14) << " ║\n";
    std::cout << "  ╠══════════╬══════════════════════════╬════════════════════════════════════════╬════════════════╣\n";
    for (const auto& dev : list) {
        std::string nameSub = dev.getDeviceName();
        if (StringUtils::utf8Width(nameSub) > 24) nameSub = nameSub.substr(0, 21) + "...";
        std::string descSub = dev.getDescription();
        if (StringUtils::utf8Width(descSub) > 38) descSub = descSub.substr(0, 35) + "...";

        std::cout << "  ║ " << StringUtils::padRight(dev.getDeviceId(), 8)
                  << " ║ " << StringUtils::padRight(nameSub, 24)
                  << " ║ " << StringUtils::padRight(descSub, 38)
                  << " ║ " << StringUtils::padRight(dev.getReportDate().toString(), 14)
                  << " ║\n";
    }
    std::cout << "  ╚══════════╩══════════════════════════╩════════════════════════════════════════╩════════════════╝\n";
}

// Bảng doanh thu toàn KTX: Tổng độ rộng = 119 ký tự
void TablePrinter::printRevenueTable(const Vector<RoomRevenueRow>& rows, const std::string& monthYear) {
    int totalWidth = 119;

    std::cout << "\n╔" << StringUtils::repeat("═", totalWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("BẢNG DOANH THU KÝ TÚC XÁ THÁNG " + monthYear, totalWidth) << "║\n";
    std::cout << "╠════════╦══════════╦═══════════╦════════════════╦═══════════════╦════════════════╦════════════════╦════════════════════╣\n";
    std::cout << "║ " << StringUtils::padRight("PHÒNG", 6)
              << " ║ " << StringUtils::padRight("LOẠI", 8)
              << " ║ " << StringUtils::padRight("SỐ NGƯỜI", 9)
              << " ║ " << StringUtils::padRight("TIỀN PHÒNG", 14)
              << " ║ " << StringUtils::padRight("PHÍ DỊCH VỤ", 13)
              << " ║ " << StringUtils::padRight("TIỀN ĐIỆN", 14)
              << " ║ " << StringUtils::padRight("TIỀN NƯỚC", 14)
              << " ║ " << StringUtils::padRight("TỔNG DOANH THU", 18) << " ║\n";
    std::cout << "╠════════╬══════════╬═══════════╬════════════════╬═══════════════╬════════════════╬════════════════╬════════════════════╣\n";

    double totalAll = 0.0;
    double totalRentAll = 0.0;
    double totalServiceAll = 0.0;
    double totalElecAll = 0.0;
    double totalWaterAll = 0.0;

    for (const auto& row : rows) {
        totalAll += row.totalRevenue;
        totalRentAll += row.rentFeeTotal;
        totalServiceAll += row.serviceFee;
        totalElecAll += row.electricCost;
        totalWaterAll += row.waterCost;

        std::cout << "║ " << StringUtils::padRight(row.roomId, 6)
                  << " ║ " << StringUtils::padRight(row.roomType, 8)
                  << " ║ " << StringUtils::padRight(std::to_string(row.occupants), 9)
                  << " ║ " << StringUtils::padRight(formatMoney(row.rentFeeTotal), 14)
                  << " ║ " << StringUtils::padRight(formatMoney(row.serviceFee), 13)
                  << " ║ " << StringUtils::padRight(formatMoney(row.electricCost), 14)
                  << " ║ " << StringUtils::padRight(formatMoney(row.waterCost), 14)
                  << " ║ " << StringUtils::padRight(formatMoney(row.totalRevenue), 18)
                  << " ║\n";
    }
    // Hàng tổng kết: 3 cột đầu gộp lại = 8 + 1 + 10 + 1 + 11 = 31 ký tự
    std::cout << "╠════════╩══════════╩═══════════╬════════════════╬═══════════════╬════════════════╬════════════════╬════════════════════╣\n";
    std::cout << "║ " << StringUtils::padRight("TỔNG CỘNG TOÀN KTX", 29)
              << " ║ " << StringUtils::padRight(formatMoney(totalRentAll), 14)
              << " ║ " << StringUtils::padRight(formatMoney(totalServiceAll), 13)
              << " ║ " << StringUtils::padRight(formatMoney(totalElecAll), 14)
              << " ║ " << StringUtils::padRight(formatMoney(totalWaterAll), 14)
              << " ║ " << StringUtils::padRight(formatMoney(totalAll), 18) << " ║\n";
    std::cout << "╚═══════════════════════════════╩════════════════╩═══════════════╩════════════════╩════════════════╩════════════════════╝\n";
    std::cout << ">> TỔNG DOANH THU KÝ TÚC XÁ THÁNG " << monthYear << ": " << formatMoney(totalAll) << "\n";
}

void TablePrinter::printSingleRoomRevenue(const RoomRevenueRow& row, const std::string& monthYear) {
    int innerWidth = 72;
    std::cout << "\n╔" << StringUtils::repeat("═", innerWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("PHIẾU TÍNH DOANH THU CHI TIẾT PHÒNG " + row.roomId + " (THÁNG " + monthYear + ")", innerWidth) << "║\n";
    std::cout << "╠" << StringUtils::repeat("═", innerWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight("Mã phòng", 28) << ": " << StringUtils::padRight(row.roomId + " (Loại: " + row.roomType + ")", 40) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("Số người đang ở", 28) << ": " << StringUtils::padRight(std::to_string(row.occupants) + "/" + std::to_string(row.maxCapacity) + " sinh viên", 40) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("Giá thuê cơ bản", 28) << ": " << StringUtils::padRight(formatMoney(row.basePrice) + " / tháng", 40) << " ║\n";
    if (row.roomType == "VIP") {
        std::cout << "║ " << StringUtils::padRight("Phí dịch vụ VIP", 28) << ": " << StringUtils::padRight(formatMoney(row.serviceFee) + " / tháng", 40) << " ║\n";
    }
    std::cout << "╠" << StringUtils::repeat("═", innerWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight("1. Tiền phòng từ hợp đồng", 28) << ": " << StringUtils::padRight(formatMoney(row.rentFeeTotal), 40) << " ║\n";
    if (row.roomType == "VIP") {
        std::cout << "║ " << StringUtils::padRight("2. Phí dịch vụ cộng thêm", 28) << ": " << StringUtils::padRight(formatMoney(row.serviceFee), 40) << " ║\n";
    }
    std::cout << "║ " << StringUtils::padRight("3. Tiền điện sinh hoạt", 28) << ": " << StringUtils::padRight(formatMoney(row.electricCost), 40) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("4. Tiền nước sinh hoạt", 28) << ": " << StringUtils::padRight(formatMoney(row.waterCost), 40) << " ║\n";
    std::cout << "╠" << StringUtils::repeat("═", innerWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight(">> TỔNG DOANH THU CỦA PHÒNG", 28) << ": " << StringUtils::padRight(formatMoney(row.totalRevenue), 40) << " ║\n";
    if (row.occupants > 0) {
        double avg = row.totalRevenue / row.occupants;
        std::cout << "║ " << StringUtils::padRight(">> Bình quân mỗi bạn nộp", 28) << ": " << StringUtils::padRight(formatMoney(avg) + " / người", 40) << " ║\n";
    } else {
        std::cout << "║ " << StringUtils::padCenter("(Phòng hiện đang trống - Chưa phát sinh thu từ sinh viên)", 70) << " ║\n";
    }
    std::cout << "╚" << StringUtils::repeat("═", innerWidth) << "╝\n";
}

// Báo cáo chi tiết doanh thu tiền phòng hàng tháng theo từng hợp đồng & phân bổ loại phòng
void TablePrinter::printDetailedMonthlyRevenueReport(const Vector<Rent*>& rents, const Vector<Room*>& rooms) {
    int totalWidth = 102;

    std::cout << "\n╔" << StringUtils::repeat("═", totalWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("BÁO CÁO DOANH THU TIỀN PHÒNG HÀNG THÁNG (BẢN CHI TIẾT)", totalWidth) << "║\n";
    std::cout << "╚" << StringUtils::repeat("═", totalWidth) << "╝\n";

    // Lọc các hợp đồng đang hiệu lực
    Vector<Rent*> activeRents;
    double totalRevenue = 0.0;
    double standardRev = 0.0;
    double vipRev = 0.0;
    int stdStudents = 0;
    int vipStudents = 0;

    for (Rent* r : rents) {
        if (r != nullptr && r->isActive()) {
            activeRents.push_back(r);
            totalRevenue += r->getRentFee();
            if (r->getRoom() != nullptr && r->getRoom()->getRoomType() == "VIP") {
                vipRev += r->getRentFee();
                vipStudents++;
            } else {
                standardRev += r->getRentFee();
                stdStudents++;
            }
        }
    }

    std::cout << "\n--- PHẦN 1: BẢNG CHI TIẾT TỪNG HỢP ĐỒNG ĐANG PHÁT SINH DOANH THU ---\n";
    if (activeRents.empty()) {
        std::cout << "(Hiện tại chưa có hợp đồng nào đang hiệu lực phát sinh doanh thu)\n";
    } else {
        std::cout << "╔═════╦════════╦══════════╦════════════════════════╦════════╦══════════╦════════════════════╦══════════╗\n";
        std::cout << "║ " << StringUtils::padRight("STT", 3)
                  << " ║ " << StringUtils::padRight("MÃ HĐ", 6)
                  << " ║ " << StringUtils::padRight("MÃ SV", 8)
                  << " ║ " << StringUtils::padRight("HỌ VÀ TÊN SINH VIÊN", 22)
                  << " ║ " << StringUtils::padRight("PHÒNG", 6)
                  << " ║ " << StringUtils::padRight("LOẠI", 8)
                  << " ║ " << StringUtils::padRight("TIỀN THUÊ / THÁNG", 18)
                  << " ║ " << StringUtils::padRight("KỲ HẠN", 8) << " ║\n";
        std::cout << "╠═════╬════════╬══════════╬════════════════════════╬════════╬══════════╬════════════════════╬══════════╣\n";

        for (size_t i = 0; i < activeRents.size(); ++i) {
            Rent* r = activeRents[i];
            std::string sId = r->getTenant() ? r->getTenant()->getId() : "N/A";
            std::string sName = r->getTenant() ? r->getTenant()->getFullName() : "N/A";
            if (StringUtils::utf8Width(sName) > 22) sName = sName.substr(0, 19) + "...";
            std::string rId = r->getRoom() ? r->getRoom()->getId() : "N/A";
            std::string rType = r->getRoom() ? r->getRoom()->getRoomType() : "STANDARD";
            std::string periodStr = EnumHelper::periodToString(r->getRentalPeriod());

            std::cout << "║ " << StringUtils::padRight(std::to_string(i + 1), 3)
                      << " ║ " << StringUtils::padRight(r->getId(), 6)
                      << " ║ " << StringUtils::padRight(sId, 8)
                      << " ║ " << StringUtils::padRight(sName, 22)
                      << " ║ " << StringUtils::padRight(rId, 6)
                      << " ║ " << StringUtils::padRight(rType, 8)
                      << " ║ " << StringUtils::padRight(formatMoney(r->getRentFee()), 18)
                      << " ║ " << StringUtils::padRight(periodStr, 8) << " ║\n";
        }
        std::cout << "╚═════╩════════╩══════════╩════════════════════════╩════════╩══════════╩════════════════════╩══════════╝\n";
    }

    std::cout << "\n--- PHẦN 2: CƠ CẤU DOANH THU THEO LOẠI PHÒNG (STANDARD vs VIP) ---\n";
    int totalRooms = static_cast<int>(rooms.size());
    int stdRooms = 0, vipRooms = 0;
    for (const Room* rm : rooms) {
        if (rm != nullptr) {
            if (rm->getRoomType() == "VIP") vipRooms++;
            else stdRooms++;
        }
    }
    double stdPct = totalRevenue > 0 ? (standardRev * 100.0 / totalRevenue) : 0.0;
    double vipPct = totalRevenue > 0 ? (vipRev * 100.0 / totalRevenue) : 0.0;

    std::cout << "╔════════════╦══════════╦══════════════╦════════════════════╦════════════════════╗\n";
    std::cout << "║ " << StringUtils::padRight("LOẠI PHÒNG", 10)
              << " ║ " << StringUtils::padRight("SỐ PHÒNG", 8)
              << " ║ " << StringUtils::padRight("SỐ SINH VIÊN", 12)
              << " ║ " << StringUtils::padRight("DOANH THU / THÁNG", 18)
              << " ║ " << StringUtils::padRight("TỶ TRỌNG (%)", 18) << " ║\n";
    std::cout << "╠════════════╬══════════╬══════════════╬════════════════════╬════════════════════╣\n";
    std::cout << "║ " << StringUtils::padRight("STANDARD", 10)
              << " ║ " << StringUtils::padRight(std::to_string(stdRooms), 8)
              << " ║ " << StringUtils::padRight(std::to_string(stdStudents) + " sinh viên", 12)
              << " ║ " << StringUtils::padRight(formatMoney(standardRev), 18)
              << " ║ " << StringUtils::padRight(std::to_string(static_cast<int>(stdPct + 0.5)) + " %", 18) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("VIP", 10)
              << " ║ " << StringUtils::padRight(std::to_string(vipRooms), 8)
              << " ║ " << StringUtils::padRight(std::to_string(vipStudents) + " sinh viên", 12)
              << " ║ " << StringUtils::padRight(formatMoney(vipRev), 18)
              << " ║ " << StringUtils::padRight(std::to_string(static_cast<int>(vipPct + 0.5)) + " %", 18) << " ║\n";
    std::cout << "╠════════════╩══════════╩══════════════╬════════════════════╬════════════════════╣\n";
    std::cout << "║ " << StringUtils::padRight("TỔNG CỘNG DOANH THU THUÊ PHÒNG", 36)
              << " ║ " << StringUtils::padRight(formatMoney(totalRevenue), 18)
              << " ║ " << StringUtils::padRight("100 %", 18) << " ║\n";
    std::cout << "╚══════════════════════════════════════╩════════════════════╩════════════════════╝\n";

    std::cout << "\n--- PHẦN 3: TỔNG HỢP CÁC CHỈ SỐ TÀI CHÍNH KÝ TÚC XÁ ---\n";
    int totalOccupants = static_cast<int>(activeRents.size());
    double avgPerStudent = totalOccupants > 0 ? (totalRevenue / totalOccupants) : 0.0;
    double avgPerRoom = totalRooms > 0 ? (totalRevenue / totalRooms) : 0.0;
    double yearlyProjected = totalRevenue * 12.0;

    int cardWidth = 76;
    std::cout << "╔" << StringUtils::repeat("═", cardWidth) << "╗\n";
    std::cout << "║" << StringUtils::padCenter("TỔNG KẾT TÀI CHÍNH TOÀN KÝ TÚC XÁ", cardWidth) << "║\n";
    std::cout << "╠" << StringUtils::repeat("═", cardWidth) << "╣\n";
    std::cout << "║ " << StringUtils::padRight("1. Tổng doanh thu tiền phòng thực thu", 44) << ": " << StringUtils::padRight(formatMoney(totalRevenue) + " / tháng", 28) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("2. Dự kiến doanh thu cả năm học (12 tháng)", 44) << ": " << StringUtils::padRight(formatMoney(yearlyProjected) + " / năm", 28) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("3. Tổng số hợp đồng hiệu lực đang thu tiền", 44) << ": " << StringUtils::padRight(std::to_string(totalOccupants) + " hợp đồng", 28) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("4. Doanh thu bình quân trên mỗi sinh viên", 44) << ": " << StringUtils::padRight(formatMoney(avgPerStudent) + " / SV / tháng", 28) << " ║\n";
    std::cout << "║ " << StringUtils::padRight("5. Doanh thu bình quân trên mỗi phòng KTX", 44) << ": " << StringUtils::padRight(formatMoney(avgPerRoom) + " / phòng / tháng", 28) << " ║\n";
    std::cout << "╚" << StringUtils::repeat("═", cardWidth) << "╝\n";
}
