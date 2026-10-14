#ifndef STUDENT_H
#define STUDENT_H

#include "model/Human.h"
#include <string>

class Rent; // Forward declaration

/**
 * @brief Lớp Sinh viên (Khớp 100% với sơ đồ XML).
 */
class Student : public Human {
private:
    std::string studentId;
    std::string className;
    std::string faculty;
    std::string cohort;
    Rent* rentInfo; // Khớp XML: - rentInfo: Rent*

public:
    Student();
    Student(const std::string& id, const std::string& name, bool g,
            const std::string& birth, const std::string& p, const std::string& em,
            const std::string& addr, const std::string& cName,
            const std::string& fac, const std::string& coh, Rent* r = nullptr);
    virtual ~Student();

    // Getters & Setters theo đúng sơ đồ XML
    std::string getStudentId() const;
    std::string getClass() const;     // Khớp XML: + getClass()
    std::string getFaculty() const;   // Khớp XML: + getFaculty()
    std::string getCohort() const;    // Khớp XML: + getCohort()
    Rent* getRent() const;            // Khớp XML: + getRent(): Rent*
    void setRent(Rent* rent);         // Khớp XML: + setRent(Rent* rent): void

    void setStudentId(const std::string& id);
    void setClass(const std::string& cName);
    void setFaculty(const std::string& fac);
    void setCohort(const std::string& coh);

    // IEntity
    std::string getId() const override;
    void print() const override;
    std::string serialize() const override;
    static Student deserialize(const std::string& line);
};

#endif // STUDENT_H
