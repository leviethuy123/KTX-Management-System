#ifndef STAFF_H
#define STAFF_H

#include "model/Human.h"
#include <string>

/**
 * @brief Lớp Cán bộ KTX (Khớp sơ đồ XML).
 */
class Staff : public Human {
private:
    std::string employeeId;
    std::string department;
    std::string position;

public:
    Staff();
    Staff(const std::string& id, const std::string& name, bool g,
          const std::string& birth, const std::string& p, const std::string& em,
          const std::string& addr, const std::string& dept,
          const std::string& pos);
    virtual ~Staff() = default;

    // Getters & Setters khớp XML
    std::string getEmployeeId() const;
    std::string getDepartment() const;
    void setDepartment(const std::string& newDepartment);
    std::string getPosition() const;
    void setPosition(const std::string& newPosition);

    void setEmployeeId(const std::string& id);

    // IEntity
    std::string getId() const override;
    void print() const override;
    std::string serialize() const override;
    static Staff deserialize(const std::string& line);
};

#endif // STAFF_H
