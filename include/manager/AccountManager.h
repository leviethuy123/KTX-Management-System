#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include "manager/Manager.h"
#include "Account.h"
#include <vector>
#include <string>

/**
 * @brief Lớp quản lý Tài khoản người dùng KTX (Kế thừa từ Manager<Account>).
 */
class AccountManager : public Manager<Account> {
public:
    AccountManager(const std::string& filePath = "data/accounts.json");
    virtual ~AccountManager() = default;

    // Đọc / Ghi file JSON
    void loadFromFile() override;
    void saveToFile() const override;

    // Xác thực đăng nhập & Quản lý bảo mật
    Account* authenticate(const std::string& username, const std::string& pass);
    Account* findByUsername(const std::string& username) const;
    Account* findByOwnerId(const std::string& ownerId) const;
    Account* createAccount(const std::string& accId, const std::string& username,
                           const std::string& password, Role role,
                           const std::string& ownerId);
};

#endif // ACCOUNT_MANAGER_H
