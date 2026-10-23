#ifndef DORM_MANAGER_H
#define DORM_MANAGER_H

#include "Managers.h"

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
    void handleRentManagement();
    void handleBillingManagement();
    void handleReportManagement();
    void handleAccountManagement();
    void handleChangePassword();

    static void pauseScreen();
    static std::string inputString(const std::string& prompt);
    static int inputInt(const std::string& prompt);
    static double inputDouble(const std::string& prompt);

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
