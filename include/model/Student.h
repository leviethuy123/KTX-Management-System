#ifndef STUDENT_H
#define STUDENT_H

#include "model/Human.h"
#include <string>

class Student : public Human {
private:
    std::string studentId;   
    std::string className;   
    std::string faculty;     
    std::string cohort;   

public:
    Student();
    Student(const std::string& id, const std::string& name, Gender g,
            const Date& birth, const std::string& p, const std::string& em,
            const std::string& addr, const std::string& cName,
            const std::string& fac, const std::string& coh);

    
    std::string getStudentId() const;
    std::string getClassName() const;
    std::string getFaculty() const;
    std::string getCohort() const;

    void setStudentId(const std::string& id);
    void setClassName(const std::string& cName);
    void setFaculty(const std::string& fac);
    void setCohort(const std::string& coh);

    
    std::string getId() const override;
    void print() const override;
    std::string serialize() const override;


    static Student deserialize(const std::string& line);
};

#endif 
