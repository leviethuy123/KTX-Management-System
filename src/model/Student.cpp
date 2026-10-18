#include "model/Student.h"
#include <iostream>
#include <iomanip>

Student::Student()
    : Human(), studentId(""), className(""), faculty(""), cohort(""), rentInfo(nullptr) {}

Student::Student(const std::string& id, const std::string& name, bool g,
                 const std::string& birth, const std::string& p, const std::string& em,
                 const std::string& addr, const std::string& cName,
                 const std::string& fac, const std::string& coh, Rent* r)
    : Human(name, g, birth, p, em, addr),
      studentId(id), className(cName), faculty(fac), cohort(coh), rentInfo(r) {}

Student::~Student() {
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

nlohmann::json Student::toJson() const {
    nlohmann::json j;
    j["studentId"] = studentId;
    j["fullName"] = fullName;
    j["gender"] = gender;
    j["birthYear"] = birthYear;
    j["phone"] = phone;
    j["email"] = email;
    j["address"] = address;
    j["className"] = className;
    j["faculty"] = faculty;
    j["cohort"] = cohort;
    return j;
}

std::string Student::serialize() const {
    return toJson().dump(2);
}

Student Student::fromJson(const nlohmann::json& j) {
    std::string id = j.value("studentId", "");
    std::string name = j.value("fullName", "");
    bool g = j.value("gender", true);
    std::string birth = j.value("birthYear", "");
    std::string phone = j.value("phone", "");
    std::string email = j.value("email", "");
    std::string addr = j.value("address", "");
    std::string cName = j.value("className", "");
    std::string fac = j.value("faculty", "");
    std::string coh = j.value("cohort", "");

    return Student(id, name, g, birth, phone, email, addr, cName, fac, coh);
}

Student Student::deserialize(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j);
    } catch (...) {
        return Student();
    }
}
