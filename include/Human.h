#ifndef HUMAN_H
#define HUMAN_H

#include "Common.h"

class Rent; // Forward declaration tránh phụ thuộc vòng

// =========================================================================
// 1. LỚP CƠ SỞ TRỪU TƯỢNG CON NGƯỜI (HUMAN)
// =========================================================================

class Human : public IEntity {
protected:
    std::string fullName;
    Gender gender;
    std::string birthYear;
    std::string phone;
    std::string email;
    std::string address;

public:
    Human();
    Human(const std::string& name, Gender g, const std::string& birth,
          const std::string& p, const std::string& em, const std::string& addr);
    virtual ~Human() = default;

    std::string getFullName() const;
    Gender getGender() const;
    std::string getBirthYear() const;
    std::string getPhone() const;
    std::string getEmail() const;
    std::string getAddress() const;

    void setFullName(const std::string& name);
    void setGender(Gender g);
    void setBirthYear(const std::string& birth);
    void setPhone(const std::string& p);
    void setEmail(const std::string& em);
    void setAddress(const std::string& addr);

    virtual std::string getId() const override = 0;
    virtual void print() const override = 0;
    virtual nlohmann::json toJson() const override = 0;
};

// =========================================================================
// 2. LỚP SINH VIÊN (STUDENT - KẾ THỪA HUMAN)
// =========================================================================

class Student : public Human {
private:
    std::string studentId;
    std::string className;
    std::string faculty;
    std::string cohort;
    Rent* rentInfo;

public:
    Student();
    Student(const std::string& id, const std::string& name, bool g,
            const std::string& birth, const std::string& p, const std::string& em,
            const std::string& addr, const std::string& cName,
            const std::string& fac, const std::string& coh, Rent* r = nullptr);
    virtual ~Student();

    std::string getStudentId() const;
    std::string getClass() const;
    std::string getFaculty() const;
    std::string getCohort() const;
    Rent* getRent() const;
    void setRent(Rent* rent);

    void setStudentId(const std::string& id);
    void setClass(const std::string& cName);
    void setFaculty(const std::string& fac);
    void setCohort(const std::string& coh);

    std::string getId() const override;
    void print() const override;
    nlohmann::json toJson() const override;
    std::string serialize() const override;

    static Student fromJson(const nlohmann::json& j);
    static Student deserialize(const std::string& jsonStr);
};

// =========================================================================
// 3. LỚP CÁN BỘ QUẢN LÝ KTX (STAFF - KẾ THỪA HUMAN)
// =========================================================================

class Staff : public Human {
private:
    std::string employeeId;
    std::string department;
    std::string position;

public:
    Staff();
    Staff(const std::string& id, const std::string& name, bool g,
          const std::string& birth, const std::string& p, const std::string& em,
          const std::string& addr, const std::string& dept, const std::string& pos);
    virtual ~Staff() = default;

    std::string getEmployeeId() const;
    std::string getDepartment() const;
    std::string getPosition() const;

    void setEmployeeId(const std::string& id);
    void setDepartment(const std::string& dept);
    void setPosition(const std::string& pos);

    std::string getId() const override;
    void print() const override;
    nlohmann::json toJson() const override;
    std::string serialize() const override;

    static Staff fromJson(const nlohmann::json& j);
    static Staff deserialize(const std::string& jsonStr);
};

#endif // HUMAN_H
