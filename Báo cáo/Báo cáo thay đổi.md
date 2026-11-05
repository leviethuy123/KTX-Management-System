# 📋 BÁO CÁO THAY ĐỔI

> **Dự án:** Quản lý Ký túc xá Sinh viên (Hệ thống PBL2 - C++17 OOP)  
> **Thời điểm cập nhật:** 05/11/2026 (ICT)  

---

## 🎯 1. TỔNG QUAN CÁC MỤC TIÊU NÂNG CẤP HỆ THỐNG
Trong phiên làm việc toàn diện này, hệ thống đã được tái cấu trúc, nâng cấp giao diện và hoàn thiện nghiệp vụ chuyên sâu bám sát các yêu cầu thực tế:
1. **Tách biệt nghiệp vụ Phân bổ & Xếp chỗ** (`handleAllocationManagement`) khỏi **Quản lý Hợp đồng thuê** (`handleContractManagement`): Rõ ràng về mặt quy trình quản trị thực tế của KTX.
2. **Bẫy lỗi dữ liệu tức thì (`try-catch`)**: Không cho phép ngầm định giá trị mặc định khi người dùng nhập sai. Mọi dữ liệu không hợp lệ đều được cảnh báo chi tiết và yêu cầu người dùng nhập lại ngay lập tức.
3. **Đóng khung giao diện hộp nét đôi (`╔══╗`, `║`, `╚══╝`) & Xóa màn hình (`clearScreen`)**: Toàn bộ 11 menu lựa chọn và các bảng biểu hiển thị đều được đồng bộ khung viền đôi chuyên nghiệp, xóa sạch màn hình khi chuyển menu.
4. **Chuẩn hóa canh chỉnh tiếng Việt UTF-8 bằng hàm tính Offset (`namespace StringUtils`)**:
   - Khắc phục triệt để hiện tượng xô lệch đường viền phải (`║`) do các hàm căn lề chuẩn C++ (`std::setw`) đếm theo **byte** thay vì **độ rộng cột hiển thị** của ký tự tiếng Việt có dấu.
   - Cung cấp các hàm: `utf8Width()`, `utf8Offset()`, `padRight()`, `padLeft()`, `padCenter()`, `repeat()`, `formatMoney()`.
5. **Nâng cấp Báo cáo Doanh thu hàng tháng chi tiết đa chiều (`printDetailedMonthlyRevenueReport`)**: Thay thế báo cáo 1 dòng sơ sài bằng báo cáo 3 phần toàn diện:
   - *Phần 1:* Bảng danh sách chi tiết từng hợp đồng đang hiệu lực phát sinh doanh thu.
   - *Phần 2:* Cơ cấu doanh thu theo loại phòng (STANDARD vs VIP, số phòng, số sinh viên, doanh thu, tỷ trọng %).
   - *Phần 3:* Bảng thẻ tóm tắt các chỉ số tài chính tổng hợp toàn KTX (doanh thu thực thu, dự phóng cả năm, doanh thu bình quân/SV, doanh thu bình quân/phòng).
6. **Tính toán doanh thu chi tiết từng tháng cho từng phòng cụ thể và toàn KTX**: Tách bạch minh bạch tiền thuê phòng, phí dịch vụ VIP, tiền điện (3.000đ/kWh), tiền nước (10.000đ/m³), chia đều tiền tiện ích cho sinh viên cùng phòng.
7. **Đồng bộ hóa 10 Khoa chuẩn Đại học Bách Khoa** (`Faculty`) qua menu chọn số 1..10, loại bỏ hoàn toàn lỗi gõ tự do sai chính tả.
8. **Kiểm tra tính hợp lệ của lớp sinh hoạt & Tự động trích xuất Khóa sinh viên** (`ClassValidator`): Tự động phân tích năm khóa học (K21, K22, K23) từ 2 ký tự đầu của `className` và lưu trữ danh mục lớp vào `data/classes.json`.
9. **Xây dựng `Makefile` chuẩn quốc tế**: Hỗ trợ biên dịch tăng tiến (`build/*.o`), tự động theo dõi file header (`-MMD -MP`), tối ưu hóa tốc độ thực thi (`-O2`), hỗ trợ gỡ lỗi (`make DEBUG=1`) và tương thích đa nền tảng (Windows CMD/PowerShell & Linux/macOS).
10. **Bẫy lỗi xác thực mã liên kết tài khoản người dùng (`ownerId`)**: Rà soát tức thì tại thời điểm cấp tài khoản: kiểm tra bắt buộc mã sinh viên/cán bộ phải tồn tại trong hệ thống, kiểm tra chống cấp trùng tài khoản cho cùng một người, ngăn chặn triệt để tình trạng tài khoản mồ côi (orphan account).

---

## 📊 2. BẢNG THỐNG KÊ CHI TIẾT CÁC FILE THAY ĐỔI

| STT | Tệp tin (File Path) | Trọng tâm nâng cấp kỹ thuật |
| :---: | :--- | :--- |
| **1** | `include/TablePrinter.h` | Khai báo `namespace StringUtils` (xử lý UTF-8 width, offset, padding, format tiền); khai báo báo cáo doanh thu chi tiết `printDetailedMonthlyRevenueReport`. |
| **2** | `src/TablePrinter.cpp` | Cài đặt `StringUtils`; căn chỉnh lại độ rộng toàn bộ bảng (`printRevenueTable`, `printStudents`, `printRooms`, `printRents`, `printStaffs`, `printAccounts`, `printBrokenDevices`, `printRoomAllocationMap`, `printSingleRoomRevenue`); triển khai báo cáo 3 phần. |
| **3** | `include/Common.h` | Ném ngoại lệ `std::invalid_argument` trong `EnumHelper`; Khai báo lớp `Faculty` (10 khoa chuẩn) và `ClassValidator`; Khai báo `Date::tryParse`. |
| **4** | `src/Common.cpp` | Cài đặt `Faculty` (menu đóng khung viền đôi, ánh xạ 1..10), `ClassValidator` (tính khóa, rà soát lớp), `Date::tryParse`. |
| **5** | `include/manager/DormManager.h` | Khai báo `clearScreen()`; 11 hàm nhập liệu bẫy lỗi an toàn (`inputGender`, `inputRole`, `inputPeriod`,...); tách 3 menu nghiệp vụ độc lập. |
| **6** | `src/manager/DormManager.cpp` | Đóng khung nét đôi toàn bộ 11 menu; xóa màn hình linh hoạt; tích hợp `try-catch` lặp lại; gọi `TablePrinter::printDetailedMonthlyRevenueReport`. |
| **7** | `include/manager/RentManager.h` | Khai báo bổ sung `transferRoom()` (chuyển phòng) và `extendRent()` (gia hạn hợp đồng). |
| **8** | `src/manager/RentManager.cpp` | Cài đặt `transferRoom()` với đầy đủ kiểm tra ràng buộc (sức chứa, giới tính, trạng thái phòng, đồng bộ con trỏ hai chiều). |
| **9** | `include/manager/RoomManager.h` | Khai báo `calculateRoomMonthlyRevenue`, in phiếu doanh thu và in bảng doanh thu toàn KTX. |
| **10** | `src/manager/RoomManager.cpp` | Cài đặt tính doanh thu; chuẩn hóa khung nét đôi cho `printOccupancyReport()`, `printRoomBill()` và `printAvailableRoom()` bằng `StringUtils`. |
| **11** | `src/Human.cpp` | Cập nhật hàm `Student::setClass()` để tự động kích hoạt tính lại khóa học (`cohort`) qua `ClassValidator::extractCohort()`. |
| **12** | `src/Room.cpp` | Tinh chỉnh hiển thị và bẫy lỗi danh mục thiết bị hỏng. |
| **13** | `Makefile` | **(Tệp mới)** Hệ thống build tự động: biên dịch tăng tiến, cờ `-O2`, cờ debug `-g`, theo dõi header `-MMD -MP`, hỗ trợ lệnh `clean`, `run`, `rebuild`, `help`. |
| **14** | `CHUC_NANG_CAC_HAM.md` | Cập nhật tài liệu kỹ thuật chi tiết toàn bộ các hàm mới, bảng tra cứu và hướng dẫn luồng dữ liệu. |
| **15** | `README.md` | Bổ sung hướng dẫn sử dụng Makefile và mô tả các chức năng nâng cấp mới. |
| **16** | `.gitignore` | Bổ sung bỏ qua thư mục `build/`, file sơ đồ `*.drawio.xml` và thư mục kỹ năng nội bộ `.agent/`. |

---

## 🔍 3. CHI TIẾT CÁC ĐIỂM SÁNG KỸ THUẬT NỔI BẬT

### 🌟 3.1. Giải quyết triệt để lỗi lệch viền console tiếng Việt (`namespace StringUtils`)
- **Nguyên nhân gốc rễ:** Mỗi ký tự tiếng Việt có dấu (ví dụ: `Ả, Ế, Ô, Đ, Ư, Ờ`) trong bảng mã UTF-8 chiếm từ 2 đến 3 bytes trên bộ nhớ, nhưng trên màn hình console monospace nó chỉ chiếm **đúng 1 cột hiển thị**. Khi dùng `std::setw(W)`, C++ trừ đi số bytes chứ không phải số cột, khiến các dòng chứa tiếng Việt bị hụt khoảng trắng, làm đường viền phải (`║`) bị thụt lùi nhấp nhô.
- **Giải pháp xử lý:**
  - `StringUtils::utf8Width(str)`: Duyệt qua các byte mở đầu UTF-8 (`(c & 0xC0) != 0x80`) để đếm chính xác số cột hiển thị.
  - `StringUtils::utf8Offset(str)`: Tính độ chênh lệch: $\text{Offset} = \text{byte\_length} - \text{visual\_width}$.
  - `StringUtils::padRight(str, visualWidth)`: Bổ sung khoảng trắng dựa trên `visualWidth - utf8Width(str)`, bảo đảm dòng in ra màn hình luôn thẳng tắp đến từng pixel.
  - Áp dụng trên 100% các bảng biểu trong `TablePrinter.cpp` và `RoomManager.cpp`.

---

### 🌟 3.2. Nâng cấp Báo cáo Doanh thu hàng tháng chi tiết (`printDetailedMonthlyRevenueReport`)
Thay thế dòng chữ sơ sài cũ bằng báo cáo 3 tầng phân tích chuyên sâu:
1. **Phần 1 - Bảng hợp đồng chi tiết:** Liệt kê toàn bộ các hợp đồng đang có hiệu lực (`ACTIVE`), hiển thị Mã hợp đồng, Mã sinh viên, Họ tên, Phòng, Loại phòng, Giá thuê/tháng và Kỳ hạn thuê.
2. **Phần 2 - Cơ cấu doanh thu theo loại phòng:** Phân tích đối sánh giữa phòng **STANDARD** và phòng **VIP**: số phòng, số sinh viên đang ở, tổng doanh thu từng loại phòng và tỷ trọng phần trăm đóng góp.
3. **Phần 3 - Thẻ chỉ số tài chính KTX:** Bảng tóm tắt các KPI quan trọng:
   - Tổng doanh thu thực thu hàng tháng.
   - Doanh thu dự phóng cả năm học (12 tháng).
   - Tổng số hợp đồng hiệu lực đang đóng tiền.
   - Doanh thu bình quân trên mỗi sinh viên (VNĐ/SV/tháng).
   - Doanh thu bình quân trên mỗi phòng KTX (VNĐ/phòng/tháng).

---

### 🌟 3.3. Tách biệt Phân bổ Xếp chỗ & Quản lý Hợp đồng thuê
- **Menu [3] - Phân bổ & Xếp chỗ Phòng KTX (`handleAllocationManagement`):**
  - Xem sơ đồ phân bổ trực quan phòng ở & danh sách sinh viên trong từng phòng.
  - Phân bổ / Xếp phòng mới cho sinh viên (kiểm tra sức chứa tối đa, kiểm tra tính tương thích giới tính nam/nữ, trạng thái phòng).
  - Chuyển phòng ở (`transferRoom`): Di dời sinh viên sang phòng mới, tự động giải phóng chỗ phòng cũ và cập nhật giá thuê tương ứng.
  - Trả phòng KTX: Kết thúc hợp đồng, cập nhật trạng thái phòng.
- **Menu [4] - Quản lý Hợp đồng thuê KTX (`handleContractManagement`):**
  - Xem danh sách hợp đồng (bảng đẹp mắt).
  - Tra cứu hợp đồng theo mã hợp đồng hoặc mã sinh viên.
  - Gia hạn hợp đồng (`extendRent`): Cho phép chọn thêm 1, 3, 6 hoặc 12 tháng.
  - Cảnh báo các hợp đồng sắp hết hạn (trong vòng 30 ngày tới).

---

### 🌟 3.4. Bẫy lỗi dữ liệu tức thì (`try-catch`) & Chuẩn hóa Khoa / Lớp
- **Không ngầm định giá trị:** Các hàm trong `EnumHelper` ném ngoại lệ `std::invalid_argument` khi người dùng nhập sai, được bao bọc trong vòng lặp `while (true)` tại `DormManager.cpp` để thông báo lỗi rõ ràng và yêu cầu nhập lại ngay.
- **Menu Khoa 1..10 (`Faculty`):** Hiển thị danh sách 10 khoa chuẩn của trường, chọn theo số thứ tự 1..10, loại bỏ hoàn toàn tình trạng sai lệch chính tả.
- **Kiểm định lớp & Tách khóa (`ClassValidator`):** Tự động bóc tách 2 ký tự đầu để xác định khóa sinh viên (ví dụ `22T_DT1` -> `K22`), đồng thời duy trì danh sách lớp hợp lệ trong `data/classes.json`.

---

### 🌟 3.5. Makefile biên dịch tự động, tăng tiến và gỡ lỗi
- **Incremental Build:** Đưa các tệp đối tượng vào `build/*.o`, chỉ biên dịch lại file vừa chỉnh sửa.
- **Header Dependency Tracking:** Sử dụng cờ `-MMD -MP` tạo tệp `.d` tự động biên dịch lại khi file header `.h` thay đổi.
- **Tùy chọn cấu hình:** Mặc định chạy `-O2`, bật debug bằng `mingw32-make DEBUG=1` (`-g -O0 -DDEBUG`).

---

## 🧪 4. KẾT QUẢ KIỂM THỬ THỰC TẾ
1. **Biên dịch dự án:** Biên dịch hoàn tất bằng lệnh `mingw32-make` với **0 lỗi, 0 cảnh báo (Zero Warnings / Zero Errors)**.
2. **Kiểm tra độ thẳng hàng (Visual Width Check):**
   - Đã kiểm thử tự động toàn bộ 18 khối bảng hiển thị trong console.
   - Bảng Báo cáo tỷ lệ lấp đầy (`printOccupancyReport`): Độ rộng 100 ký tự trên mọi dòng.
   - Bảng Doanh thu KTX tháng (`printRevenueTable`): Độ rộng 121 ký tự trên mọi dòng.
   - Thẻ Phiếu báo tiền phòng (`printRoomBill`): Độ rộng 76 ký tự trên mọi dòng.
   - Báo cáo doanh thu chi tiết (`printDetailedMonthlyRevenueReport`): Độ rộng 104, 82, 78 ký tự trên các phân đoạn, khớp chính xác từng ký tự biên `║`.
3. **Tính toàn vẹn dữ liệu:** Toàn bộ dữ liệu trong thư mục `data/` được giữ nguyên vẹn, không có bất kỳ xáo trộn dữ liệu gốc nào.