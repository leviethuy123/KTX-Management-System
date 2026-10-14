#include "model/Student.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

Student::Student()
    : Human(), studentId(""), className(""), faculty(""), cohort(""), rentInfo(nullptr) {}

Student::Student(const std::string& id, const std::string& name, bool g,
                 const std::string& birth, const std::string& p, const std::string& em,
                 const std::string& addr, const std::string& cName,
                 const std::string& fac, const std::string& coh, Rent* r)
    : Human(name, g, birth, p, em, addr),
      studentId(id), className(cName), faculty(fac), cohort(coh), rentInfo(r) {}

Student::~Student() {
    // Không delete rentInfo vì Rent được quản lý bởi RentManager
    rentInfo = nullptr;
}

std::string Student::getStudentId() const { return studentId; }
std::string Student::getClass() const { return className; }
std::string Student::getFaculty() const { return faculty; }
std::string Student::getCohort() const { return cohort; }
Rent* Student::getRent() const { return rentInfo; }

void Student::setRent(Rent* rent) { rentInfo = rent; }
void Student::setStudentId(const std::string& id) { studentId = id; }
void Student::setClass(const std::string& cName) { className = cName; }
void Student::setFaculty(const std::string& fac) { faculty = fac; }
void Student::setCohort(const std::string& coh) { cohort = coh; }

std::string Student::getId() const {
    return studentId;
}

void Student::print() const {
    std::cout << "[SV] " << std::left << std::setw(8) << studentId
              << " | " << std::setw(20) << fullName
              << " | " << std::setw(6) << (gender ? "Nam" : "Nu")
              << " | Năm sinh: " << std::setw(6) << birthYear
              << " | Lớp: " << std::setw(10) << className
              << " | Khoa: " << std::setw(18) << faculty
              << " | Khóa: " << std::setw(6) << cohort
              << " | SĐT: " << phone << std::endl;
}

std::string Student::serialize() const {
    std::ostringstream oss;
    oss << studentId << "|"
        << fullName << "|"
        << (gender ? "1" : "0") << "|"
        << birthYear << "|"
        << phone << "|"
        << email << "|"
        << address << "|"
        << className << "|"
        << faculty << "|"
        << cohort;
    return oss.str();
}

Student Student::deserialize(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 10) {
        return Student();
    }

    std::string id = tokens[0];
    std::string name = tokens[1];
    bool g = (tokens[2] == "1" || tokens[2] == "Nam");
    std::string birth = tokens[3];
    std::string phone = tokens[4];
    std::string email = tokens[5];
    std::string addr = tokens[6];
    std::string cName = tokens[7];
    std::string fac = tokens[8];
    std::string coh = tokens[9];

    return Student(id, name, g, birth, phone, email, addr, cName, fac, coh);
}
