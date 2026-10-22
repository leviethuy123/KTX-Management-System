#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include "manager/Manager.h"
#include "model/Account.h"
#include <string>
#include <vector>

/**
 * @brief Lớp quản lý Tài khoản người dùng (Kế thừa từ Manager<Account>).
 * Khớp chuẩn thiết kế trong sơ đồ XML PBL2.
 */
class AccountManager : public Manager<Account> {
public:
    AccountManager(const std::string& filePath = "data/accounts.json");
    virtual ~AccountManager() = default;

    // Đọc / Ghi file JSON
    void loadFromFile() override;
    void saveToFile() const override;

    // Nghiệp vụ khớp sơ đồ XML
    Account* authenticate(const std::string& username, const std::string& pass);
    Account* findByUsername(const std::string& username) const;

    // Các nghiệp vụ mở rộng
    Account* findByOwnerId(const std::string& ownerId) const;
    Account* createAccount(const std::string& accId, const std::string& username,
                           const std::string& password, Role role,
                           const std::string& ownerId);
};

#endif // ACCOUNT_MANAGER_H
