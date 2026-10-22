#ifndef STAFF_MANAGER_H
#define STAFF_MANAGER_H

#include "manager/Manager.h"
#include "model/Staff.h"
#include <vector>
#include <string>

/**
 * @brief Lớp quản lý Cán bộ KTX (Kế thừa từ Manager<Staff>).
 */
class StaffManager : public Manager<Staff> {
public:
    StaffManager(const std::string& filePath = "data/staffs.json");
    virtual ~StaffManager() = default;

    // Đọc / Ghi file JSON
    void loadFromFile() override;
    void saveToFile() const override;

    // Nghiệp vụ lọc theo phòng ban
    std::vector<Staff*> findByDepartment(const std::string& dept) const;
};

#endif // STAFF_MANAGER_H
