#include "DormManager.h"
#include <iostream>
#include <iomanip>
#include <limits>

DormManager::DormManager()
    : currentAccount(nullptr) {
    roomManage = new RoomManager("data/rooms.json");
    studentManage = new StudentManager("data/students.json");
    staffManager = new StaffManager("data/staffs.json");
    rentManager = new RentManager("data/rents.json");
    accountManager = new AccountManager("data/accounts.json");
    loadAll();
}

DormManager::~DormManager() {
    saveAll();
    delete roomManage;
    delete studentManage;
    delete staffManager;
    delete rentManager;
    delete accountManager;
}

void DormManager::loadAll() {
    studentManage->loadFromFile();
    roomManage->loadFromFile();
    staffManager->loadFromFile();
    accountManager->loadFromFile();
    rentManager->loadFromFile(*studentManage, *roomManage);
}

void DormManager::saveAll() const {
    studentManage->saveToFile();
    roomManage->saveToFile();
    staffManager->saveToFile();
    rentManager->saveToFile();
    accountManager->saveToFile();
}

bool DormManager::login(const std::string& user, const std::string& pass) {
    Account* acc = accountManager->authenticate(user, pass);
    if (acc != nullptr) {
        currentAccount = acc;
        return true;
    }
    return false;
}

void DormManager::logout() {
    if (currentAccount != nullptr) {
        std::cout << "\n[Thành công] Đã đăng xuất khỏi tài khoản '" << currentAccount->getUsername() << "'.\n";
        currentAccount = nullptr;
    }
}

Role DormManager::getCurrentUserRole() const {
    if (currentAccount != nullptr) {
        return currentAccount->getRole();
    }
    return Role::STUDENT;
}

void DormManager::pauseScreen() {
    std::cout << "\nNhấn Enter để tiếp tục...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

std::string DormManager::inputString(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    if (!std::getline(std::cin, s)) {
        return "";
    }
    if (s.size() >= 3 &&
        static_cast<unsigned char>(s[0]) == 0xEF &&
        static_cast<unsigned char>(s[1]) == 0xBB &&
        static_cast<unsigned char>(s[2]) == 0xBF) {
        s.erase(0, 3);
    }
    while (!s.empty() && (s.back() == '\r' || s.back() == ' ' || s.back() == '\t')) {
        s.pop_back();
    }
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t')) {
        s.erase(0, 1);
    }
    return s;
}

int DormManager::inputInt(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return 0;
        std::string s = inputString(prompt);
        if (s.empty()) {
            if (std::cin.eof()) return 0;
            continue;
        }
        try {
            return std::stoi(s);
        } catch (...) {
            if (std::cin.eof()) return 0;
            std::cout << "[Lỗi] Vui lòng nhập số nguyên hợp lệ!\n";
        }
    }
}

double DormManager::inputDouble(const std::string& prompt) {
    while (true) {
        if (std::cin.eof()) return 0.0;
        std::string s = inputString(prompt);
        if (s.empty()) {
            if (std::cin.eof()) return 0.0;
            continue;
        }
        try {
            return std::stod(s);
        } catch (...) {
            if (std::cin.eof()) return 0.0;
            std::cout << "[Lỗi] Vui lòng nhập số thực hợp lệ!\n";
        }
    }
}

void DormManager::runMainMenu() {
    bool running = true;
    while (running) {
        if (currentAccount == nullptr) {
            std::cout << "\n========================================================================\n";
            std::cout << "           HỆ THỐNG QUẢN LÝ KÝ TÚC XÁ SINH VIÊN (PBL2)                  \n";
            std::cout << "========================================================================\n";
            std::cout << " 1. Đăng nhập hệ thống\n";
            std::cout << " 0. Thoát chương trình & Lưu dữ liệu\n";
            std::cout << "------------------------------------------------------------------------\n";
            int choice = inputInt(">> Lựa chọn của bạn: ");

            if (choice == 1) {
                showLoginScreen();
            } else if (choice == 0) {
                std::cout << "\nĐang lưu toàn bộ dữ liệu hệ thống...\n";
                saveAll();
                std::cout << "Cảm ơn bạn đã sử dụng chương trình! Tạm biệt.\n";
                running = false;
            } else {
                std::cout << "[Cảnh báo] Lựa chọn không hợp lệ!\n";
                pauseScreen();
            }
        } else {
            switch (currentAccount->getRole()) {
                case Role::ADMIN:
                    runAdminMenu();
                    break;
                case Role::STAFF:
                    runStaffMenu();
                    break;
                case Role::STUDENT:
                    runStudentMenu();
                    break;
            }
        }
    }
}

void DormManager::showLoginScreen() {
    std::cout << "\n------------------- ĐĂNG NHẬP HỆ THỐNG -------------------\n";
    std::string user = inputString("Tên đăng nhập: ");
    std::string pass = inputString("Mật khẩu: ");

    if (login(user, pass)) {
        std::cout << "[Thành công] Đăng nhập thành công với vai trò: "
                  << EnumHelper::roleToString(currentAccount->getRole()) << "\n";
    } else {
        std::cout << "[Thất bại] Sai tên đăng nhập hoặc mật khẩu!\n";
    }
    pauseScreen();
}

void DormManager::runStudentMenu() {
    std::string ownerId = currentAccount->getOwnerId();
    Student* s = studentManage->findById(ownerId);

    std::cout << "\n========================================================================\n";
    std::cout << "               CỔNG THÔNG TIN SINH VIÊN NỘI TRÚ KÝ TÚC XÁ               \n";
    std::cout << " Xin chào: " << (s ? s->getFullName() : currentAccount->getUsername())
              << " | Mã SV: " << ownerId << "\n";
    std::cout << "========================================================================\n";
    std::cout << " 1. Xem thông tin hồ sơ cá nhân\n";
    std::cout << " 2. Xem thông tin phòng ở hiện tại & bạn cùng phòng\n";
    std::cout << " 3. Xem hợp đồng thuê phòng của tôi\n";
    std::cout << " 4. Xem phiếu báo tiền phòng & chi phí sinh hoạt tháng này\n";
    std::cout << " 5. Đổi mật khẩu tài khoản\n";
    std::cout << " 0. Đăng xuất tài khoản\n";
    std::cout << "------------------------------------------------------------------------\n";
    int choice = inputInt(">> Nhập lựa chọn: ");

    switch (choice) {
        case 1:
            std::cout << "\n--- HỒ SƠ CÁ NHÂN CỦA BẠN ---\n";
            if (s != nullptr) s->print();
            else std::cout << "Chưa có thông tin hồ sơ cho tài khoản này.\n";
            pauseScreen();
            break;
        case 2: {
            std::cout << "\n--- THÔNG TIN PHÒNG Ở HIỆN TẠI ---\n";
            if (s != nullptr && s->getRent() != nullptr && s->getRent()->isActive()) {
                Room* rm = s->getRent()->getRoom();
                if (rm != nullptr) {
                    rm->print();
                    std::cout << "\nDanh sách bạn cùng phòng:\n";
                    for (const Rent* r : rm->getActiveRent()) {
                        if (r != nullptr && r->getTenant() != nullptr) {
                            std::cout << " - " << r->getTenant()->getFullName()
                                      << " (" << r->getTenant()->getId() << ") | Lớp: "
                                      << r->getTenant()->getClass() << "\n";
                        }
                    }
                }
            } else {
                std::cout << "Bạn hiện tại chưa được xếp phòng hoặc hợp đồng đã kết thúc.\n";
            }
            pauseScreen();
            break;
        }
        case 3:
            std::cout << "\n--- HỢP ĐỒNG THUÊ PHÒNG CỦA BẠN ---\n";
            if (s != nullptr && s->getRent() != nullptr) {
                s->getRent()->print();
            } else {
                std::cout << "Bạn hiện tại không có hợp đồng thuê nào đang hiệu lực.\n";
            }
            pauseScreen();
            break;
        case 4: {
            std::cout << "\n--- PHIẾU BÁO TIỀN PHÒNG & SINH HOẠT THÁNG NÀY ---\n";
            if (s != nullptr && s->getRent() != nullptr && s->getRent()->isActive()) {
                Room* rm = s->getRent()->getRoom();
                if (rm != nullptr) {
                    double elec = inputDouble("Nhập chỉ số điện tiêu thụ của phòng tháng này (kWh): ");
                    double water = inputDouble("Nhập chỉ số nước tiêu thụ của phòng tháng này (m³): ");
                    roomManage->printRoomBill(rm->getId(), elec, water);
                }
            } else {
                std::cout << "Bạn hiện không ở trong phòng nào để xuất hóa đơn.\n";
            }
            pauseScreen();
            break;
        }
        case 5:
            handleChangePassword();
            break;
        case 0:
            logout();
            break;
        default:
            std::cout << "[Cảnh báo] Lựa chọn không hợp lệ!\n";
            pauseScreen();
            break;
    }
}

void DormManager::runStaffMenu() {
    std::string ownerId = currentAccount->getOwnerId();
    Staff* st = staffManager->findById(ownerId);

    std::cout << "\n========================================================================\n";
    std::cout << "                  BÀN LÀM VIỆC CÁN BỘ QUẢN LÝ KTX                       \n";
    std::cout << " Cán bộ: " << (st ? st->getFullName() : currentAccount->getUsername())
              << " | Bộ phận: " << (st ? st->getDepartment() : "Ban Quản lý") << "\n";
    std::cout << "========================================================================\n";
    std::cout << " 1. Quản lý Danh sách Phòng ở\n";
    std::cout << " 2. Quản lý Hồ sơ Sinh viên\n";
    std::cout << " 3. Quản lý Hợp đồng & Xếp chỗ KTX\n";
    std::cout << " 4. Tính phí & Xuất phiếu báo tiền phòng điện nước\n";
    std::cout << " 5. Báo cáo Tỷ lệ Lấp đầy & Doanh thu\n";
    std::cout << " 6. Đổi mật khẩu cá nhân\n";
    std::cout << " 0. Đăng xuất\n";
    std::cout << "------------------------------------------------------------------------\n";
    int choice = inputInt(">> Nhập lựa chọn: ");

    switch (choice) {
        case 1: handleRoomManagement(); break;
        case 2: handleStudentManagement(); break;
        case 3: handleRentManagement(); break;
        case 4: handleBillingManagement(); break;
        case 5: handleReportManagement(); break;
        case 6: handleChangePassword(); break;
        case 0: logout(); break;
        default:
            std::cout << "[Cảnh báo] Lựa chọn không hợp lệ!\n";
            pauseScreen();
            break;
    }
}

void DormManager::runAdminMenu() {
    std::cout << "\n========================================================================\n";
    std::cout << "              TRUNG TÂM QUẢN TRỊ VIÊN HỆ THỐNG KTX (ADMIN)              \n";
    std::cout << " Quản trị viên: " << currentAccount->getUsername() << "\n";
    std::cout << "========================================================================\n";
    std::cout << " 1. Quản lý Danh sách Phòng ở\n";
    std::cout << " 2. Quản lý Hồ sơ Sinh viên\n";
    std::cout << " 3. Quản lý Hợp đồng & Xếp chỗ KTX\n";
    std::cout << " 4. Quản lý Hồ sơ Cán bộ KTX\n";
    std::cout << " 5. Quản lý Hệ thống Tài khoản & Phân quyền\n";
    std::cout << " 6. Báo cáo Tổng thể (Lấp đầy phòng & Doanh thu)\n";
    std::cout << " 7. Đổi mật khẩu cá nhân\n";
    std::cout << " 0. Đăng xuất\n";
    std::cout << "------------------------------------------------------------------------\n";
    int choice = inputInt(">> Nhập lựa chọn: ");

    switch (choice) {
        case 1: handleRoomManagement(); break;
        case 2: handleStudentManagement(); break;
        case 3: handleRentManagement(); break;
        case 4: {
            std::cout << "\n--- DANH SÁCH CÁN BỘ QUẢN LÝ KTX ---\n";
            staffManager->printAll();
            pauseScreen();
            break;
        }
        case 5: handleAccountManagement(); break;
        case 6: handleReportManagement(); break;
        case 7: handleChangePassword(); break;
        case 0: logout(); break;
        default:
            std::cout << "[Cảnh báo] Lựa chọn không hợp lệ!\n";
            pauseScreen();
            break;
    }
}

void DormManager::handleRoomManagement() {
    std::cout << "\n--- [QUẢN LÝ PHÒNG Ở] ---\n";
    std::cout << " 1. Xem toàn bộ danh sách phòng\n";
    std::cout << " 2. Lọc các phòng còn chỗ trống\n";
    std::cout << " 3. Lọc phòng theo Tòa nhà\n";
    std::cout << " 4. Lọc phòng theo Loại phòng (STANDARD / VIP)\n";
    std::cout << " 5. Xem chi tiết 1 phòng theo mã\n";
    std::cout << " 0. Quay lại\n";
    int c = inputInt(">> Lựa chọn: ");

    if (c == 1) {
        std::cout << "\n=== TẤT CẢ CÁC PHÒNG TRONG KTX ===\n";
        roomManage->printAll();
    } else if (c == 2) {
        roomManage->printAvailableRoom();
    } else if (c == 3) {
        std::string bld = inputString("Nhập tên tòa nhà (ví dụ: Khu A, Khu VIP): ");
        auto list = roomManage->findByBuilding(bld);
        std::cout << "\nKết quả lọc theo tòa '" << bld << "': (" << list.size() << " phòng)\n";
        for (const Room* r : list) r->print();
    } else if (c == 4) {
        std::string type = inputString("Nhập loại phòng (STANDARD hoặc VIP): ");
        auto list = roomManage->findByType(type);
        std::cout << "\nKết quả lọc theo loại '" << type << "': (" << list.size() << " phòng)\n";
        for (const Room* r : list) r->print();
    } else if (c == 5) {
        std::string id = inputString("Nhập mã phòng: ");
        roomManage->printById(id);
    }
    pauseScreen();
}

void DormManager::handleStudentManagement() {
    std::cout << "\n--- [QUẢN LÝ SINH VIÊN] ---\n";
    std::cout << " 1. Xem danh sách tất cả sinh viên\n";
    std::cout << " 2. Tìm kiếm sinh viên theo Mã sinh viên\n";
    std::cout << " 3. Lọc sinh viên theo Lớp\n";
    std::cout << " 4. Lọc sinh viên theo Khoa\n";
    std::cout << " 5. Thêm sinh viên mới vào hệ thống\n";
    std::cout << " 0. Quay lại\n";
    int c = inputInt(">> Lựa chọn: ");

    if (c == 1) {
        std::cout << "\n=== DANH SÁCH SINH VIÊN ===\n";
        studentManage->printAll();
    } else if (c == 2) {
        std::string id = inputString("Nhập mã sinh viên: ");
        studentManage->printById(id);
    } else if (c == 3) {
        std::string lop = inputString("Nhập tên lớp: ");
        auto list = studentManage->findByClass(lop);
        std::cout << "\nDanh sách sinh viên lớp '" << lop << "':\n";
        for (const Student* s : list) s->print();
    } else if (c == 4) {
        std::string khoa = inputString("Nhập khoa: ");
        auto list = studentManage->findByFaculty(khoa);
        std::cout << "\nDanh sách sinh viên khoa '" << khoa << "':\n";
        for (const Student* s : list) s->print();
    } else if (c == 5) {
        std::cout << "\n--- NHẬP THÔNG TIN SINH VIÊN MỚI ---\n";
        std::string id = inputString("Mã SV: ");
        if (studentManage->findById(id) != nullptr) {
            std::cout << "[Lỗi] Mã sinh viên này đã tồn tại!\n";
        } else {
            std::string name = inputString("Họ và tên: ");
            int gChoice = inputInt("Giới tính (1: Nam, 2: Nữ): ");
            bool g = (gChoice == 1);
            std::string birth = inputString("Năm sinh (ví dụ: 2004): ");
            std::string lop = inputString("Lớp: ");
            std::string khoa = inputString("Khoa: ");
            std::string khoaHoc = inputString("Khóa (K22, K23,...): ");
            std::string phone = inputString("Số điện thoại: ");
            std::string email = inputString("Email: ");
            std::string address = inputString("Quê quán: ");

            Student* newS = new Student(id, name, g, birth, phone, email, address, lop, khoa, khoaHoc);
            if (studentManage->add(newS)) {
                std::cout << "[Thành công] Đã thêm sinh viên " << name << " vào hệ thống!\n";
                accountManager->createAccount("ACC_" + id, id, "123456", Role::STUDENT, id);
            }
        }
    }
    pauseScreen();
}

void DormManager::handleRentManagement() {
    std::cout << "\n--- [QUẢN LÝ HỢP ĐỒNG & XẾP CHỖ KTX] ---\n";
    std::cout << " 1. Xem tất cả hợp đồng thuê\n";
    std::cout << " 2. Đăng ký thuê phòng mới cho sinh viên (Xếp phòng)\n";
    std::cout << " 3. Thanh lý sớm hợp đồng / Trả phòng\n";
    std::cout << " 4. Kiểm tra hợp đồng sắp hết hạn (30 ngày)\n";
    std::cout << " 5. Quét và tự động giải phóng các hợp đồng quá hạn\n";
    std::cout << " 0. Quay lại\n";
    int c = inputInt(">> Lựa chọn: ");

    if (c == 1) {
        std::cout << "\n=== TOÀN BỘ HỢP ĐỒNG KTX ===\n";
        rentManager->printAll();
    } else if (c == 2) {
        std::cout << "\n--- TẠO HỢP ĐỒNG THUÊ PHÒNG MỚI ---\n";
        std::string rentId = inputString("Mã hợp đồng mới (ví dụ: HD004): ");
        std::string sId = inputString("Mã sinh viên: ");
        std::string rId = inputString("Mã phòng muốn xếp: ");

        Student* s = studentManage->findById(sId);
        Room* r = roomManage->findById(rId);

        if (s == nullptr) {
            std::cout << "[Lỗi] Không tìm thấy sinh viên có mã: " << sId << "\n";
        } else if (r == nullptr) {
            std::cout << "[Lỗi] Không tìm thấy phòng có mã: " << rId << "\n";
        } else {
            int pMonths = inputInt("Thời hạn thuê (1, 3, 6, 12 tháng): ");
            Period period = EnumHelper::intToPeriod(pMonths);
            int d = inputInt("Ngày bắt đầu (1-31): ");
            int m = inputInt("Tháng bắt đầu (1-12): ");
            int y = inputInt("Năm bắt đầu: ");

            rentManager->createRent(rentId, s, r, period, Date(d, m, y), r->getBasePrice());
        }
    } else if (c == 3) {
        std::string rentId = inputString("Nhập mã hợp đồng cần thanh lý: ");
        int d = inputInt("Ngày trả phòng (1-31): ");
        int m = inputInt("Tháng trả phòng (1-12): ");
        int y = inputInt("Năm trả phòng: ");
        rentManager->terminateRent(rentId, Date(d, m, y));
    } else if (c == 4) {
        int d = inputInt("Nhập ngày kiểm tra mốc hiện tại (1-31): ");
        int m = inputInt("Nhập tháng kiểm tra (1-12): ");
        int y = inputInt("Nhập năm kiểm tra: ");
        rentManager->printExpiringRents(Date(d, m, y), 30);
    } else if (c == 5) {
        int d = inputInt("Nhập ngày mốc cần quét quá hạn (1-31): ");
        int m = inputInt("Nhập tháng mốc: ");
        int y = inputInt("Nhập năm mốc: ");
        int count = rentManager->checkAndExpireRents(Date(d, m, y));
        std::cout << "Tổng số hợp đồng đã quá hạn được giải phóng: " << count << "\n";
    }
    pauseScreen();
}

void DormManager::handleBillingManagement() {
    std::cout << "\n--- [XUẤT HÓA ĐƠN TIỀN PHÒNG & ĐIỆN NƯỚC] ---\n";
    std::string rId = inputString("Nhập mã phòng cần tính phí (ví dụ: P102, P201): ");
    Room* r = roomManage->findById(rId);
    if (r == nullptr) {
        std::cout << "[Lỗi] Không tìm thấy phòng: " << rId << "\n";
    } else {
        double elec = inputDouble("Nhập số điện tiêu thụ trong tháng (kWh): ");
        double water = inputDouble("Nhập số nước tiêu thụ trong tháng (m³): ");
        roomManage->printRoomBill(rId, elec, water);
    }
    pauseScreen();
}

void DormManager::handleReportManagement() {
    std::cout << "\n--- [BÁO CÁO THỐNG KÊ KÝ TÚC XÁ] ---\n";
    std::cout << " 1. Báo cáo tỷ lệ lấp đầy phòng KTX\n";
    std::cout << " 2. Báo cáo tổng doanh thu tiền phòng hàng tháng\n";
    std::cout << " 0. Quay lại\n";
    int c = inputInt(">> Lựa chọn: ");

    if (c == 1) {
        roomManage->printOccupancyReport();
    } else if (c == 2) {
        double rev = rentManager->calculateTotalMonthlyRevenue();
        std::cout << "\n=========================================================\n";
        std::cout << "       BÁO CÁO DOANH THU TIỀN PHÒNG HÀNG THÁNG           \n";
        std::cout << "=========================================================\n";
        std::cout << "-> Tổng doanh thu ước tính: " << std::fixed << std::setprecision(0)
                  << rev << " VNĐ / tháng\n";
        std::cout << "=========================================================\n";
    }
    pauseScreen();
}

void DormManager::handleAccountManagement() {
    std::cout << "\n--- [QUẢN LÝ TÀI KHOẢN NGƯỜI DÙNG] ---\n";
    std::cout << " 1. Xem danh sách toàn bộ tài khoản\n";
    std::cout << " 2. Cấp tài khoản mới\n";
    std::cout << " 3. Khóa / Mở khóa tài khoản\n";
    std::cout << " 0. Quay lại\n";
    int c = inputInt(">> Lựa chọn: ");

    if (c == 1) {
        std::cout << "\n=== TẤT CẢ TÀI KHOẢN TRONG HỆ THỐNG ===\n";
        accountManager->printAll();
    } else if (c == 2) {
        std::string accId = inputString("Mã tài khoản (ví dụ: ACC006): ");
        std::string user = inputString("Tên đăng nhập: ");
        std::string pass = inputString("Mật khẩu khởi tạo: ");
        int rChoice = inputInt("Vai trò (1: STUDENT, 2: STAFF, 3: ADMIN): ");
        Role r = Role::STUDENT;
        if (rChoice == 2) r = Role::STAFF;
        else if (rChoice == 3) r = Role::ADMIN;
        std::string owner = inputString("Mã liên kết (Mã SV/Mã Cán bộ): ");

        accountManager->createAccount(accId, user, pass, r, owner);
    } else if (c == 3) {
        std::string user = inputString("Nhập tên đăng nhập cần đổi trạng thái: ");
        Account* acc = accountManager->findByUsername(user);
        if (acc == nullptr) {
            std::cout << "[Lỗi] Không tìm thấy tài khoản: " << user << "\n";
        } else {
            acc->setStatus(!acc->getStatus());
            std::cout << "[Thành công] Tài khoản '" << user << "' hiện đã "
                      << (acc->getStatus() ? "KÍCH HOẠT" : "BỊ KHÓA") << ".\n";
        }
    }
    pauseScreen();
}

void DormManager::handleChangePassword() {
    std::cout << "\n--- ĐỔI MẬT KHẨU TÀI KHOẢN ---\n";
    std::string oldPass = inputString("Nhập mật khẩu hiện tại: ");
    if (!currentAccount->verifyPassword(oldPass)) {
        std::cout << "[Lỗi] Mật khẩu hiện tại không chính xác!\n";
    } else {
        std::string newPass = inputString("Nhập mật khẩu mới: ");
        std::string confirm = inputString("Xác nhận mật khẩu mới: ");
        if (newPass != confirm) {
            std::cout << "[Lỗi] Mật khẩu xác nhận không trùng khớp!\n";
        } else {
            currentAccount->changePassword(newPass);
            std::cout << "[Thành công] Đổi mật khẩu thành công!\n";
        }
    }
    pauseScreen();
}
