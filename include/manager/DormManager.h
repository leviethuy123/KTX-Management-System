#ifndef DORM_MANAGER_H
#define DORM_MANAGER_H

#include "manager/StudentManager.h"
#include "manager/RoomManager.h"
#include "manager/StaffManager.h"
#include "manager/RentManager.h"
#include "manager/AccountManager.h"
#include "model/Account.h"
#include "common/Enums.h"
#include <string>

/**
 * @brief Lớp điều phối trung tâm toàn bộ hệ thống Ký túc xá (Facade Design Pattern).
 * Khớp chuẩn 100% sơ đồ thiết kế XML PBL2.
 * Điều phối dữ liệu giữa các Manager và cung cấp giao diện Console Menu phân quyền.
 */
class DormManager {
private:
    RoomManager* roomManage;
    StudentManager* studentManage;
    RentManager* rentManager;
    StaffManager* staffManager;
    AccountManager* accountManager;
    Account* currentAccount;

    // Các hàm xử lý giao diện Menu phân quyền nội bộ
    void showLoginScreen();
    void runAdminMenu();
    void runStaffMenu();
    void runStudentMenu();

    // Các hàm nghiệp vụ tương tác chi tiết
    void handleRoomManagement();
    void handleStudentManagement();
    void handleRentManagement();
    void handleBillingManagement();
    void handleReportManagement();
    void handleAccountManagement();
    void handleChangePassword();

    // Tiện ích nhập xuất dữ liệu an toàn
    static void pauseScreen();
    static std::string inputString(const std::string& prompt);
    static int inputInt(const std::string& prompt);
    static double inputDouble(const std::string& prompt);

public:
    DormManager();
    virtual ~DormManager();

    // Quản lý nạp & lưu trữ dữ liệu
    void loadAll();
    void saveAll() const;

    // Nghiệp vụ khớp sơ đồ XML PBL2
    bool login(const std::string& user, const std::string& pass);
    void logout();
    Role getCurrentUserRole() const;
    void runMainMenu();
};

#endif // DORM_MANAGER_H
