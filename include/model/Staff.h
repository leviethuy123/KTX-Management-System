#ifndef STAFF_H
#define STAFF_H

#include "model/Human.h"
#include <string>


class Staff : public Human {
private:
    std::string employeeId; 
    std::string department;  
    std::string position;   

public:
    Staff();
    Staff(const std::string& id, const std::string& name, Gender g,
          const Date& birth, const std::string& p, const std::string& em,
          const std::string& addr, const std::string& dept,
          const std::string& pos);

  
    std::string getEmployeeId() const;
    std::string getDepartment() const;
    std::string getPosition() const;

    void setEmployeeId(const std::string& id);
    void setDepartment(const std::string& dept);
    void setPosition(const std::string& pos);

    
    std::string getId() const override;
    void print() const override;
    std::string serialize() const override;

    static Staff deserialize(const std::string& line);
};

#endif 
