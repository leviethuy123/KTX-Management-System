#include "Account.h"

Account::Account()
    : accountId(""), username(""), passwordHash(""),
      role(Role::STUDENT), ownerId(""), status(true) {}

Account::Account(const std::string& accId, const std::string& user,
                 const std::string& pass, Role r, const std::string& owner,
                 bool st)
    : accountId(accId), username(user), passwordHash(hashPassword(pass)),
      role(r), ownerId(owner), status(st) {}

std::string Account::getId() const { return accountId; }
std::string Account::getAccountId() const { return accountId; }
std::string Account::getUsername() const { return username; }
std::string Account::getPasswordHash() const { return passwordHash; }
Role Account::getRole() const { return role; }
std::string Account::getOwnerId() const { return ownerId; }
bool Account::getStatus() const { return status; }

void Account::setAccountId(const std::string& id) { accountId = id; }
void Account::setUsername(const std::string& user) { username = user; }
void Account::setPasswordHash(const std::string& hash) { passwordHash = hash; }
void Account::setRole(Role r) { role = r; }
void Account::setOwnerId(const std::string& owner) { ownerId = owner; }
void Account::setStatus(bool st) { status = st; }

std::string Account::hashPassword(const std::string& password) {
    unsigned long hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << hash;
    return ss.str();
}

bool Account::verifyPassword(const std::string& password) const {
    return passwordHash == hashPassword(password) || passwordHash == password;
}

void Account::changePassword(const std::string& newPass) {
    passwordHash = hashPassword(newPass);
}

void Account::print() const {
    std::cout << "[TÀI KHOẢN " << std::left << std::setw(6) << accountId << "] "
              << "Username: " << std::setw(14) << username
              << " | Vai trò: " << std::setw(8) << EnumHelper::roleToString(role)
              << " | Liên kết ID: " << std::setw(8) << (ownerId.empty() ? "(None)" : ownerId)
              << " | Trạng thái: " << (status ? "HOAT DONG" : "BI KHOA") << std::endl;
}

nlohmann::json Account::toJson() const {
    return {
        {"accountId", accountId},
        {"username", username},
        {"passwordHash", passwordHash},
        {"role", EnumHelper::roleToString(role)},
        {"ownerId", ownerId},
        {"status", status}
    };
}

Account Account::fromJson(const nlohmann::json& j) {
    Account acc;
    acc.accountId = j.value("accountId", "");
    acc.username = j.value("username", "");
    acc.passwordHash = j.value("passwordHash", "");
    acc.role = EnumHelper::stringToRole(j.value("role", "STUDENT"));
    acc.ownerId = j.value("ownerId", "");
    acc.status = j.value("status", true);
    return acc;
}

Account Account::deserialize(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return fromJson(j);
    } catch (...) {
        return Account();
    }
}
