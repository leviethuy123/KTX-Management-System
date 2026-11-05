#ifndef DORM_MANAGER_H
#define DORM_MANAGER_H

#include "manager/RoomManager.h"
#include "manager/StudentManager.h"
#include "manager/RentManager.h"
#include "manager/StaffManager.h"
#include "manager/AccountManager.h"
#include "TablePrinter.h"

/**
 * @brief Lớp điều phối trung tâm toàn bộ hệ thống Ký túc xá (Facade Design Pattern).
 * Đóng gói toàn bộ các Manager và cung cấp giao diện tương tác Menu Console phân quyền.
 */
class DormManager {
private:
    RoomManager* roomManage;
    StudentManager* studentManage;
    RentManager* rentManager;
    StaffManager* staffManager;
    AccountManager* accountManager;
    Account* currentAccount;

    void showLoginScreen();
    void runAdminMenu();
    void runStaffMenu();
    void runStudentMenu();

    void handleRoomManagement();
    void handleStudentManagement();
    void handleAllocationManagement(); // Nghiệp vụ Phân bổ & Xếp chỗ phòng KTX
    void handleContractManagement();   // Nghiệp vụ Quản lý Hợp đồng KTX
    void handleRevenueManagement();    // Nghiệp vụ Quản lý Doanh thu & Hóa đơn
    void handleReportManagement();
    void handleAccountManagement();
    void handleChangePassword();

    static void printHeaderInfo();
    static void clearScreen();
    static void pauseScreen();
    static std::string inputString(const std::string& prompt);
    static int inputInt(const std::string& prompt);
    static double inputDouble(const std::string& prompt);
    static std::string inputNonEmpty(const std::string& prompt);
    static Date inputValidDate(const std::string& prompt, const Date& defaultDate = Date(5, 11, 2026));
    static std::string inputValidPhone(const std::string& prompt);
    static std::string inputValidEmail(const std::string& prompt);
    static std::string inputValidClassName(const std::string& prompt);
    static std::string inputFacultyChoice();
    static Gender inputGender(const std::string& prompt);
    static Role inputRole(const std::string& prompt);
    static Period inputPeriod(const std::string& prompt);
    static RoomStatus inputRoomStatus(const std::string& prompt);

public:
    DormManager();
    virtual ~DormManager();

    void loadAll();
    void saveAll() const;

    bool login(const std::string& user, const std::string& pass);
    void logout();
    Role getCurrentUserRole() const;
    void runMainMenu();
};

#endif // DORM_MANAGER_H
