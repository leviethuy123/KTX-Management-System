#ifndef STUDENT_H
#define STUDENT_H

#include "model/Human.h"
#include <string>
#include <nlohmann/json.hpp>

class Rent; // Forward declaration

/**
 * @brief Lớp Sinh viên (Khớp sơ đồ XML, hỗ trợ lưu trữ JSON).
 */
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

    // Getters & Setters
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

    // IEntity & JSON
    std::string getId() const override;
    void print() const override;
    nlohmann::json toJson() const override;
    std::string serialize() const override;

    static Student fromJson(const nlohmann::json& j);
    static Student deserialize(const std::string& jsonStr);
};

#endif // STUDENT_H
