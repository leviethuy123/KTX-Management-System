#include "manager/AccountManager.h"
#include <fstream>
#include <iostream>

AccountManager::AccountManager(const std::string& filePath)
    : Manager<Account>(filePath) {}

void AccountManager::loadFromFile() {
    clear();
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Thong bao] Khong the mo file: " << dataFilePath << ". Bat dau voi danh sach trong.\n";
        return;
    }

    try {
        nlohmann::json jArr;
        file >> jArr;
        for (const auto& item : jArr) {
            Account acc = Account::fromJson(item);
            list.push_back(new Account(acc));
        }
        std::cout << "[Thanh cong] Da nap " << list.size() << " tai khoan tu " << dataFilePath << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Loi] Parse JSON tai khoan that bai: " << e.what() << "\n";
    }
}

void AccountManager::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        std::cout << "[Loi] Khong the mo file de ghi: " << dataFilePath << "\n";
        return;
    }

    nlohmann::json jArr = nlohmann::json::array();
    for (const Account* acc : list) {
        if (acc != nullptr) {
            jArr.push_back(acc->toJson());
        }
    }

    file << jArr.dump(2);
    std::cout << "[Thanh cong] Da luu " << list.size() << " tai khoan vao " << dataFilePath << "\n";
}

Account* AccountManager::authenticate(const std::string& username, const std::string& pass) {
    Account* acc = findByUsername(username);
    if (acc == nullptr) {
        std::cout << "[Xac thuc that bai] Khong ton tai tai khoan voi ten dang nhap: '" << username << "'\n";
        return nullptr;
    }

    if (!acc->getStatus()) {
        std::cout << "[Xac thuc that bai] Tai khoan '" << username << "' dang bi khoa!\n";
        return nullptr;
    }

    if (!acc->verifyPassword(pass)) {
        std::cout << "[Xac thuc that bai] Sai mat khau!\n";
        return nullptr;
    }

    std::cout << "[Xac thuc thanh cong] Xin chao " << username
              << " (" << EnumHelper::roleToString(acc->getRole()) << ")!\n";
    return acc;
}

Account* AccountManager::findByUsername(const std::string& username) const {
    for (Account* acc : list) {
        if (acc != nullptr && acc->getUsername() == username) {
            return acc;
        }
    }
    return nullptr;
}

Account* AccountManager::findByOwnerId(const std::string& ownerId) const {
    for (Account* acc : list) {
        if (acc != nullptr && acc->getOwnerId() == ownerId) {
            return acc;
        }
    }
    return nullptr;
}

Account* AccountManager::createAccount(const std::string& accId, const std::string& username,
                                      const std::string& password, Role role,
                                      const std::string& ownerId) {
    if (findById(accId) != nullptr) {
        std::cout << "[Loi] Ma tai khoan '" << accId << "' da ton tai!\n";
        return nullptr;
    }

    if (findByUsername(username) != nullptr) {
        std::cout << "[Loi] Ten dang nhap '" << username << "' da duoc su dung!\n";
        return nullptr;
    }

    Account* newAcc = new Account(accId, username, password, role, ownerId, true);
    list.push_back(newAcc);
    std::cout << "[Thanh cong] Da tao tai khoan '" << username << "' ("
              << EnumHelper::roleToString(role) << ")\n";
    return newAcc;
}
