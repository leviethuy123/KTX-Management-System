#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "manager/StudentManager.h"
#include "manager/RoomManager.h"
#include "manager/StaffManager.h"
#include "manager/RentManager.h"

int main() {
#ifdef _WIN32
    // Thiet lap bang ma UTF-8 cho Windows Console de hien thi tieng Viet co dau
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::cout << "=========================================================\n";
    std::cout << "   HE THONG QUAN LY KTX - DEMO GIAI DOAN 2: MANAGERS     \n";
    std::cout << "=========================================================\n\n";

    // 1. Khoi tao cac Manager chuyen biet
    StudentManager studentMgr("data/students.json");
    RoomManager roomMgr("data/rooms.json");
    StaffManager staffMgr("data/staffs.json");
    RentManager rentMgr("data/rents.json");

    // 2. Nap du lieu tu cac file JSON
    std::cout << "--- 1. NAP DU LIEU TU CAC FILE JSON ---\n";
    studentMgr.loadFromFile();
    roomMgr.loadFromFile();
    staffMgr.loadFromFile();
    // Dong bo hop dong voi sinh vien va phong
    rentMgr.loadFromFile(studentMgr, roomMgr);

    // 3. Kiem thu cac phuong thuc ke thua tu Manager<T> (Template)
    std::cout << "\n--- 2. IN TOAN BO DANH SACH (MANAGER<T>::printAll) ---\n";
    std::cout << "\n[DANH SACH PHONG]:\n";
    roomMgr.printAll();

    std::cout << "\n[DANH SACH SINH VIEN]:\n";
    studentMgr.printAll();

    std::cout << "\n[DANH SACH CAN BO]:\n";
    staffMgr.printAll();

    std::cout << "\n[DANH SACH HOP DONG]:\n";
    rentMgr.printAll();

    // 4. Kiem thu nghiep vu dac thu cua tung Manager
    std::cout << "\n--- 3. KIEM THU NGIEP VU TIM KIEM & LOC ---\n";
    std::cout << "\n> Loc cac phong con trong:\n";
    roomMgr.printAvailableRoom();

    std::cout << "\n> Tim sinh vien thuoc lop '22T_DT1':\n";
    auto svLop = studentMgr.findByClass("22T_DT1");
    for (auto* s : svLop) {
        s->print();
    }

    // 5. Kiem thu Tao hop dong moi thong qua RentManager
    std::cout << "\n--- 4. TAO HOP DONG THUE MOI ---\n";
    Student* svChuaCoPhong = studentMgr.findById("SV103");
    Room* phongTrong = roomMgr.findById("P102");

    if (svChuaCoPhong != nullptr && phongTrong != nullptr) {
        std::cout << "Tien hanh xep sinh vien " << svChuaCoPhong->getFullName()
                  << " vao phong " << phongTrong->getId() << ":\n";
        
        Rent* hdMoi = rentMgr.createRent(
            "HD003",
            svChuaCoPhong,
            phongTrong,
            Period::SIX_MONTHS,
            Date(1, 10, 2026),
            phongTrong->getBasePrice()
        );

        if (hdMoi != nullptr) {
            std::cout << "Thong tin phong P102 sau khi them sinh vien:\n";
            phongTrong->print();
        }
    }

    // 6. Kiem thu Thanh ly hop dong som
    std::cout << "\n--- 5. THANH LY HOP DONG SOM ---\n";
    std::cout << "Thanh ly hop dong HD001 vao ngay 15/11/2026:\n";
    rentMgr.terminateRent("HD001", Date(15, 11, 2026));

    Room* p101 = roomMgr.findById("P101");
    if (p101 != nullptr) {
        std::cout << "Trang thai phong P101 sau khi sinh vien tra phong:\n";
        p101->print();
    }

    // 7. Luu du lieu nguoc lai vao cac file JSON
    std::cout << "\n--- 6. DONG BO & LUU DU LIEU NGUOC LAI FILE JSON ---\n";
    studentMgr.saveToFile();
    roomMgr.saveToFile();
    rentMgr.saveToFile();

    std::cout << "\n=========================================================\n";
    std::cout << "  HOAN THANH KIEM THU TOAN BO GIAI DOAN 2 THANH CONG!     \n";
    std::cout << "=========================================================\n";

    return 0;
}
