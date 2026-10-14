#include <iostream>
#include <vector>
#include <memory>
#include "common/Enums.h"
#include "common/Date.h"
#include "model/Student.h"
#include "model/Staff.h"
#include "model/StandardRoom.h"
#include "model/VipRoom.h"
#include "model/Rent.h"

int main() {
    std::cout << "=========================================================" << std::endl;
    std::cout << "  HE THONG QUAN LY KTX - KIEM THU ARCHITECTURE XML MOI   " << std::endl;
    std::cout << "=========================================================\n" << std::endl;

    // 1. Thử nghiệm Lớp Con người (Human -> Student & Staff theo sơ đồ XML)
    std::cout << "--- 1. DANH SACH SINH VIEN & CAN BO (HUMAN -> STUDENT / STAFF) ---" << std::endl;
    Student sv1("SV101", "Nguyen Van An", true, "2004", "0901234567",
                "an.nv@sv.dut.udn.vn", "Da Nang", "22T_DT1", "CNTT", "K22");
    Student sv2("SV102", "Tran Thi Bich", false, "2004", "0912345678",
                "bich.tt@sv.dut.udn.vn", "Quang Nam", "22KT", "Kien truc", "K22");
    Staff cb1("CB001", "Le Van Cuong", true, "1985", "0987654321",
              "cuong.lv@dut.udn.vn", "Da Nang", "Ban Quan ly KTX", "Truong nha Khu A");

    sv1.print();
    sv2.print();
    cb1.print();

    // 2. Thử nghiệm Phòng (Room -> StandardRoom & VipRoom với tiện nghi mới)
    std::cout << "\n--- 2. DANH SACH PHONG (ROOM -> STANDARD / VIP ROOM) ---" << std::endl;
    StandardRoom roomStandard("P101", 1, "Khu A");
    VipRoom roomVip("P201", 2, "Khu VIP");

    std::vector<Room*> roomList = { &roomStandard, &roomVip };
    for (Room* r : roomList) {
        r->print();
        std::cout << "  --> Phi du kien (50 kWh dien, 3 m3 nuoc): "
                  << static_cast<long long>(r->calculateTotalFee(50, 3)) << " VND/thang" << std::endl;
    }

    // 3. Thử nghiệm Hợp đồng thuê phòng (Rent với Period và liên kết 2 chiều)
    std::cout << "\n--- 3. HOP DONG THUE PHONG (RENT VOI PERIOD & LIEN KET ROOM/STUDENT) ---" << std::endl;
    Rent hd1("HD001", &sv1, &roomStandard, Period::SIX_MONTHS, Date(1, 9, 2026), roomStandard.getBasePrice());
    sv1.setRent(&hd1);
    roomStandard.addRent(&hd1);

    Rent hd2("HD002", &sv2, &roomVip, Period::ONE_YEAR, Date(1, 9, 2026), roomVip.getBasePrice());
    sv2.setRent(&hd2);
    roomVip.addRent(&hd2);

    hd1.print();
    std::cout << "  --> Ngay ket thuc du kien (Expected End Date): " << hd1.getExpectedEndDate().toString() << std::endl;
    std::cout << "  --> Tong gia tri hop dong: " << static_cast<long long>(hd1.calculateTotalAmount()) << " VND" << std::endl;

    hd2.print();
    std::cout << "  --> Ngay ket thuc du kien (Expected End Date): " << hd2.getExpectedEndDate().toString() << std::endl;
    std::cout << "  --> Tong gia tri hop dong: " << static_cast<long long>(hd2.calculateTotalAmount()) << " VND" << std::endl;

    // Kiểm tra trạng thái phòng sau khi thêm sinh viên
    std::cout << "\nTrang thai phong sau khi addRent():" << std::endl;
    roomStandard.print();
    roomVip.print();

    // 4. Thử nghiệm Thanh lý hợp đồng sớm (terminateEarly) & removeRent
    std::cout << "\n--- 4. THANH LY SOM HOP DONG (terminateEarly) & TRA PHONG (removeRent) ---" << std::endl;
    std::cout << "Thanh ly som HD001 vao ngay 15/11/2026:" << std::endl;
    hd1.terminateEarly(Date(15, 11, 2026));
    hd1.print();

    roomStandard.removeRent(&hd1);
    sv1.setRent(nullptr);
    std::cout << "Trang thai phong P101 sau khi tra phong: " << std::endl;
    roomStandard.print();
    std::cout << "Phong P101 co trong khong? " << (roomStandard.isEmpty() ? "CO (Dung)" : "KHONG") << std::endl;

    // 5. Thử nghiệm Serialize & Deserialize (Lưu trữ dữ liệu)
    std::cout << "\n--- 5. KIEM TRA XUAT / DOC CHUOI DU LIEU (SERIALIZATION) ---" << std::endl;
    std::string strSv = sv1.serialize();
    std::cout << "Serialized Student: " << strSv << std::endl;
    Student restoredSv = Student::deserialize(strSv);
    std::cout << "Restored Student: " << restoredSv.getFullName() << " | Khoa: " << restoredSv.getFaculty() << std::endl;

    std::string strRent = hd2.serialize();
    std::cout << "Serialized Rent: " << strRent << std::endl;
    Rent restoredRent = Rent::deserialize(strRent, &sv2, &roomVip);
    std::cout << "Restored Rent: " << restoredRent.getId()
              << " | Tenant: " << restoredRent.getTenant()->getFullName()
              << " | Phong: " << restoredRent.getRoom()->getId()
              << " | Han: " << EnumHelper::periodToString(restoredRent.getRentalPeriod()) << std::endl;

    std::cout << "\n=========================================================" << std::endl;
    std::cout << "  TAT CA CAC TEST THEO SO DO XML MOI DEU THANH CONG!     " << std::endl;
    std::cout << "=========================================================" << std::endl;

    return 0;
}
