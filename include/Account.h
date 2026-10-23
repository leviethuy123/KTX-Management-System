#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Common.h"

class Account : public IEntity {
private:
    std::string accountId;
    std::string username;
    std::string passwordHash;
    Role role;
    std::string ownerId;
    bool status;

public:
    Account();
    Account(const std::string& accId, const std::string& user,
            const std::string& pass, Role r, const std::string& owner,
            bool st = true);
    virtual ~Account() = default;

    std::string getId() const override;
    std::string getAccountId() const;
    std::string getUsername() const;
    std::string getPasswordHash() const;
    Role getRole() const;
    std::string getOwnerId() const;
    bool getStatus() const;

    void setAccountId(const std::string& id);
    void setUsername(const std::string& user);
    void setPasswordHash(const std::string& hash);
    void setRole(Role r);
    void setOwnerId(const std::string& owner);
    void setStatus(bool st);

    bool verifyPassword(const std::string& password) const;
    void changePassword(const std::string& newPass);
    static std::string hashPassword(const std::string& password);

    void print() const override;
    nlohmann::json toJson() const override;
    static Account fromJson(const nlohmann::json& j);
    static Account deserialize(const std::string& jsonStr);
};

#endif // ACCOUNT_H
