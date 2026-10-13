#include "model/Student.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

Student::Student()
    : Human(), studentId(""), className(""), faculty(""), cohort("") {}

Student::Student(const std::string& id, const std::string& name, Gender g,
                 const Date& birth, const std::string& p, const std::string& em,
                 const std::string& addr, const std::string& cName,
                 const std::string& fac, const std::string& coh)
    : Human(name, g, birth, p, em, addr),
      studentId(id), className(cName), faculty(fac), cohort(coh) {}

std::string Student::getStudentId() const { return studentId; }
std::string Student::getClassName() const { return className; }
std::string Student::getFaculty() const { return faculty; }
std::string Student::getCohort() const { return cohort; }

void Student::setStudentId(const std::string& id) { studentId = id; }
void Student::setClassName(const std::string& cName) { className = cName; }
void Student::setFaculty(const std::string& fac) { faculty = fac; }
void Student::setCohort(const std::string& coh) { cohort = coh; }

std::string Student::getId() const {
    return studentId;
}

void Student::print() const {
    std::cout << "[SV] " << std::left << std::setw(8) << studentId
              << " | " << std::setw(20) << fullName
              << " | " << std::setw(6) << EnumHelper::genderToString(gender)
              << " | " << std::setw(10) << birthDate.toString()
              << " | Lớp: " << std::setw(10) << className
              << " | Khoa: " << std::setw(15) << faculty
              << " | SĐT: " << phone << std::endl;
}

std::string Student::serialize() const {
    std::ostringstream oss;
    oss << studentId << "|"
        << fullName << "|"
        << EnumHelper::genderToString(gender) << "|"
        << birthDate.toString() << "|"
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
    Gender g = EnumHelper::stringToGender(tokens[2]);
    Date birth = Date::fromString(tokens[3]);
    std::string phone = tokens[4];
    std::string email = tokens[5];
    std::string addr = tokens[6];
    std::string cName = tokens[7];
    std::string fac = tokens[8];
    std::string coh = tokens[9];

    return Student(id, name, g, birth, phone, email, addr, cName, fac, coh);
}
