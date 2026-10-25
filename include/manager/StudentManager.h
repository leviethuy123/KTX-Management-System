#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "manager/Manager.h"
#include "Human.h"
#include <vector>
#include <string>

/**
 * @brief Lớp quản lý Sinh viên KTX (Kế thừa từ Manager<Student>).
 */
class StudentManager : public Manager<Student> {
public:
    StudentManager(const std::string& filePath = "data/students.json");
    virtual ~StudentManager() = default;

    // Đọc / Ghi file JSON
    void loadFromFile() override;
    void saveToFile() const override;

    // Nghiệp vụ tìm kiếm đặc thù
    std::vector<Student*> findByClass(const std::string& className) const;
    std::vector<Student*> findByFaculty(const std::string& faculty) const;
};

#endif // STUDENT_MANAGER_H
