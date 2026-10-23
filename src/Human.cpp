#include "Human.h"
#include "Rent.h"

// =========================================================================
// 1. CÀI ĐẶT HUMAN
// =========================================================================

Human::Human()
    : fullName(""), gender(Gender::OTHER), birthYear("2000"),
      phone(""), email(""), address("") {}

Human::Human(const std::string& name, Gender g, const std::string& birth,
             const std::string& p, const std::string& em, const std::string& addr)
    : fullName(name), gender(g), birthYear(birth),
      phone(p), email(em), address(addr) {}

std::string Human::getFullName() const { return fullName; }
Gender Human::getGender() const { return gender; }
std::string Human::getBirthYear() const { return birthYear; }
std::string Human::getPhone() const { return phone; }
std::string Human::getEmail() const { return email; }
std::string Human::getAddress() const { return address; }

void Human::setFullName(const std::string& name) { fullName = name; }
void Human::setGender(Gender g) { gender = g; }
void Human::setBirthYear(const std::string& birth) { birthYear = birth; }
void Human::setPhone(const std::string& p) { phone = p; }
void Human::setEmail(const std::string& em) { email = em; }
void Human::setAddress(const std::string& addr) { address = addr; }

// =========================================================================
// 2. CÀI ĐẶT STUDENT
// =========================================================================

Student::Student()
    : Human(), studentId(""), className(""), faculty(""), cohort(""), rentInfo(nullptr) {}

Student::Student(const std::string& id, const std::string& name, Gender g,
                 const std::string& p, const std::string& e, const std::string& b,
                 const std::string& a, const std::string& cName,
                 const std::string& fac, const std::string& coh)
    : Human(name, g, p, e, b, a),studentId(id), className(cName), faculty(fac), cohort(coh) {}
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

std::string Student::getId() const { return studentId; }

void Student::print() const {
    std::cout << "[SV] " << std::left << std::setw(8) << studentId << " | "
              << std::setw(20) << fullName << " | "
              << std::setw(6) << EnumHelper::genderToString(gender) << " | "
              << "Năm sinh: " << std::setw(6) << birthYear << " | "
              << "Lớp: " << std::setw(10) << className << " | "
              << "Khoa: " << std::setw(20) << faculty << " | "
              << "Khóa: " << std::setw(6) << cohort << " | "
              << "SĐT: " << phone << std::endl;
}

nlohmann::json Student::toJson() const {
    return {
        {"studentId", studentId},
        {"fullName", fullName},
        {"gender", gender == Gender::MALE},
        {"birthYear", birthYear},
        {"phone", phone},
        {"email", email},
        {"address", address},
        {"className", className},
        {"faculty", faculty},
        {"cohort", cohort}
    };
}

std::string Student::serialize() const {
    return toJson().dump(2);
}

Student Student::fromJson(const nlohmann::json& j) {
    Student s;
    s.studentId = j.value("studentId", "");
    s.fullName = j.value("fullName", "");
    bool isMale = j.value("gender", true);
    s.gender = isMale ? Gender::MALE : Gender::FEMALE;
    s.birthYear = j.value("birthYear", "2000");
    s.phone = j.value("phone", "");
    s.email = j.value("email", "");
    s.address = j.value("address", "");
    s.className = j.value("className", "");
    s.faculty = j.value("faculty", "");
    s.cohort = j.value("cohort", "");
    s.rentInfo = nullptr;
    return s;
}

Student Student::deserialize(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j);
    } catch (...) {
        return Student();
    }
}

// =========================================================================
// 3. CÀI ĐẶT STAFF
// =========================================================================

Staff::Staff()
    : Human(), employeeId(""), department(""), position("") {}

Staff::Staff(const std::string& id, const std::string& name, bool g,
             const std::string& birth, const std::string& p, const std::string& em,
             const std::string& addr, const std::string& dept, const std::string& pos)
    : Human(name, g ? Gender::MALE : Gender::FEMALE, birth, p, em, addr),
      employeeId(id), department(dept), position(pos) {}

std::string Staff::getEmployeeId() const { return employeeId; }
std::string Staff::getDepartment() const { return department; }
std::string Staff::getPosition() const { return position; }

void Staff::setEmployeeId(const std::string& id) { employeeId = id; }
void Staff::setDepartment(const std::string& dept) { department = dept; }
void Staff::setPosition(const std::string& pos) { position = pos; }

std::string Staff::getId() const { return employeeId; }

void Staff::print() const {
    std::cout << "[CÁN BỘ] " << std::left << std::setw(8) << employeeId << " | "
              << std::setw(20) << fullName << " | "
              << std::setw(6) << EnumHelper::genderToString(gender) << " | "
              << "Phòng ban: " << std::setw(18) << department << " | "
              << "Chức vụ: " << std::setw(16) << position << " | "
              << "SĐT: " << phone << std::endl;
}

nlohmann::json Staff::toJson() const {
    return {
        {"employeeId", employeeId},
        {"fullName", fullName},
        {"gender", gender == Gender::MALE},
        {"birthYear", birthYear},
        {"phone", phone},
        {"email", email},
        {"address", address},
        {"department", department},
        {"position", position}
    };
}

std::string Staff::serialize() const {
    return toJson().dump(2);
}

Staff Staff::fromJson(const nlohmann::json& j) {
    Staff st;
    st.employeeId = j.value("employeeId", "");
    st.fullName = j.value("fullName", "");
    bool isMale = j.value("gender", true);
    st.gender = isMale ? Gender::MALE : Gender::FEMALE;
    st.birthYear = j.value("birthYear", "1980");
    st.phone = j.value("phone", "");
    st.email = j.value("email", "");
    st.address = j.value("address", "");
    st.department = j.value("department", "");
    st.position = j.value("position", "");
    return st;
}

Staff Staff::deserialize(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j);
    } catch (...) {
        return Staff();
    }
}
