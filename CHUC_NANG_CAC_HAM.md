# 📑 TỔNG HỢP CHỨC NĂNG CÁC HÀM TRONG DỰ ÁN KTX-MANAGEMENT-SYSTEM
> **Tài liệu tham khảo nhanh phục vụ học tập và bảo vệ đồ án PBL2**  
> *Được biên soạn ngắn gọn, súc tích, phân theo từng tầng kiến trúc.*

---

## 📌 PHẦN 1: THƯ VIỆN DÙNG CHUNG (`include/common/`)

### 1. File `IEntity.h` (Interface cơ sở cho mọi thực thể)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã định danh duy nhất của thực thể (ID phòng, ID sinh viên, ID hợp đồng,...). |
| `print()` | In thông tin chi tiết của đối tượng ra màn hình Console định dạng đẹp. |
| `toJson()` | Chuyển đổi đối tượng C++ thành cấu trúc dữ liệu JSON (`nlohmann::json`). |
| `serialize()` | Chuyển đổi đối tượng thành chuỗi JSON String có thụt dòng (indent 2) để lưu file. |

---

### 2. File `Date.h` / `Date.cpp` (Xử lý Ngày - Tháng - Năm)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `Date(d, m, y)` | Khởi tạo ngày tháng năm; tự động sửa về ngày hợp lệ nếu dữ liệu sai. |
| `isValid()` | Kiểm tra ngày tháng có hợp lệ không (xét năm nhuận, số ngày trong tháng 1-12). |
| `toString()` | Xuất chuỗi định dạng chuẩn `dd/mm/yyyy` (ví dụ: `01/09/2026`). |
| `fromString(str)` | Chuyển chuỗi `dd/mm/yyyy` ngược lại thành đối tượng `Date`. |
| `addMonths(m)` | Cộng thêm `m` tháng vào ngày hiện tại (dùng để tính ngày hết hạn hợp đồng). |
| `toDays()` | Đổi ngày tháng năm thành tổng số ngày tuyệt đối tính từ năm 1900. |
| `daysBetween(other)` | Tính chính xác khoảng cách số ngày giữa 2 mốc thời gian (`other - this`). |
| `operator<, <=, >, ==` | Nạp chồng toán tử so sánh thứ tự thời gian trước/sau giữa hai ngày. |

---

### 3. File `Enums.h` (Các kiểu liệt kê & Hàm chuyển đổi chuỗi)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `genderToString / stringToGender` | Chuyển đổi qua lại giữa `Gender` (MALE, FEMALE) và chuỗi tiếng Việt. |
| `roleToString / stringToRole` | Chuyển đổi qua lại giữa vai trò `Role` (STUDENT, STAFF, ADMIN) và chuỗi. |
| `roomStatusToString / stringToRoomStatus` | Chuyển đổi trạng thái phòng (EMPTY, OCCUPIED, FULL, MAINTENANCE). |
| `rentStatusToString / stringToRentStatus` | Chuyển đổi trạng thái hợp đồng (ACTIVE, EXPIRED, TERMINATED). |
| `periodToMonths / intToPeriod` | Chuyển đổi giữa kỳ hạn hợp đồng `Period` và số tháng nguyên (1, 3, 6, 12). |

---

## 🏢 PHẦN 2: CÁC THỰC THỂ DỮ LIỆU (`include/model/`)

### 1. File `Human.h` / `Human.cpp` (Lớp cơ sở trừu tượng Con người)
*Lưu trữ thông tin cá nhân: Họ tên, giới tính, ngày sinh, số điện thoại, email, địa chỉ.*
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getFullName() / setFullName()` | Lấy hoặc cập nhật họ và tên. |
| `getPhone(), getEmail(),...` | Các Getter / Setter lấy và sửa thông tin liên lạc, quê quán. |
| `getId()` *(thuần ảo = 0)* | Bắt buộc lớp con (`Student`, `Staff`) phải tự định nghĩa mã cá nhân. |

---

### 2. File `Student.h` / `Student.cpp` (Lớp Sinh viên - Kế thừa `Human`)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã sinh viên `studentId` (ví dụ: `SV101`). |
| `getClassName(), getFaculty()` | Lấy tên lớp sinh hoạt và khoa quản lý của sinh viên. |
| `getRent() / setRent()` | Lấy hoặc liên kết con trỏ hợp đồng thuê phòng hiện tại của sinh viên. |
| `print()` | In thông tin cá nhân, mã sinh viên, lớp, khoa và trạng thái phòng ở. |
| `toJson() / fromJson()` | Đóng gói thông tin sinh viên sang JSON và trích xuất từ JSON vào đối tượng. |
| `serialize() / deserialize()` | Chuyển đổi giữa đối tượng Sinh viên và chuỗi văn bản JSON. |

---

### 3. File `Staff.h` / `Staff.cpp` (Lớp Cán bộ KTX - Kế thừa `Human`)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã cán bộ `employeeId` (ví dụ: `CB001`). |
| `getDepartment(), getPosition()` | Lấy thông tin phòng ban công tác và chức vụ của cán bộ. |
| `print()` | In thông tin hồ sơ cán bộ quản lý KTX. |
| `toJson() / fromJson()` | Đóng gói và đọc thông tin cán bộ từ định dạng JSON. |

---

### 4. File `Room.h` / `Room.cpp` (Lớp cơ sở trừu tượng Phòng KTX)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã phòng `roomId` (ví dụ: `P101`, `P201`). |
| `addRent(Rent*)` | Thêm hợp đồng sinh viên vào phòng; tự động đổi trạng thái sang `OCCUPIED` hoặc `FULL`. |
| `removeRent(Rent*)` | Gỡ hợp đồng khi sinh viên trả phòng; tự động đổi trạng thái về `EMPTY` hoặc `OCCUPIED`. |
| `isEmpty()` | Kiểm tra phòng có đang trống hoàn toàn hay không (`activeRent.empty()`). |
| `isFull()` | Kiểm tra phòng đã đủ người chưa (`activeRent.size() >= maxStudents`). |
| `calculateTotalFee()` *(thuần ảo)* | Phương thức đa hình tính tổng tiền phòng (được lớp con ghi đè). |
| `getRoomType()` *(thuần ảo)* | Trả về loại phòng (`STANDARD` hoặc `VIP`). |

---

### 5. File `StandardRoom.h` / `StandardRoom.cpp` (Phòng Tiêu chuẩn)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `calculateTotalFee(elec, water)` | **Đa hình:** $\text{Giá gốc (400k)} + (\text{Điện} \times 3.000) + (\text{Nước} \times 10.000)$. |
| `getAmenities()` | Trả về danh sách tiện ích cơ bản (quạt, bàn học, ghế). |
| `toJson() / fromJson()` | Đọc và ghi dữ liệu phòng thường cùng trang thiết bị kèm theo. |

---

### 6. File `VipRoom.h` / `VipRoom.cpp` (Phòng VIP dịch vụ cao cấp)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `calculateTotalFee(elec, water)` | **Đa hình:** $\text{Giá gốc (1.2tr)} + \text{Phí VIP (250k)} + (\text{Điện} \times 3.000) + (\text{Nước} \times 10.000)$. |
| `getAmenities()` | Trả về danh sách tiện ích VIP (điều hòa, bình nóng lạnh, wifi, dọn phòng). |
| `getServiceFee()` | Lấy mức phí dịch vụ cộng thêm của phòng VIP. |

---

### 7. File `Rent.h` / `Rent.cpp` (Hợp đồng thuê phòng KTX)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã hợp đồng `rentId` (ví dụ: `HD001`). |
| `getExpectedEndDate()` | Tính ngày kết thúc dự kiến = Ngày bắt đầu + Số tháng thuê (`startDate.addMonths()`). |
| `terminateEarly(actualDate)` | Chấm dứt hợp đồng sớm trước hạn, đổi trạng thái sang `TERMINATED`. |
| `isActive()` | Kiểm tra hợp đồng có đang hiệu lực không (`status == RentStatus::ACTIVE`). |
| `calculateTotalAmount()` | Tính tổng tiền thuê cả kỳ = `rentalPeriod * rentFee`. |
| `toJson() / fromJson()` | Lưu trữ thông tin hợp đồng liên kết giữa `studentId` và `roomId`. |

---

### 8. File `Account.h` / `Account.cpp` (Tài khoản người dùng & Bảo mật)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `getId()` | Trả về mã tài khoản `accountId` (ví dụ: `ACC001`). |
| `hashPassword(password)` | Thuật toán băm mật khẩu DJB2 sang chuỗi Hex để lưu trữ an toàn. |
| `verifyPassword(password)` | Kiểm tra mật khẩu người dùng nhập vào có khớp với mã băm hay không. |
| `changePassword(newPass)` | Đổi mật khẩu mới và tự động băm lại mật khẩu. |
| `getRole()` | Lấy quyền hạn của tài khoản (`STUDENT`, `STAFF`, hoặc `ADMIN`). |

---

## ⚙️ PHẦN 3: TẦNG QUẢN LÝ DỮ LIỆU & NGHIỆP VỤ (`include/manager/`)

### 1. File `Manager.h` (Lớp khuôn mẫu generic `Manager<T>`)
*Áp dụng C++ Template - Mẫu Repository Pattern.*
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `add(T* item)` | Thêm một đối tượng mới vào danh sách; tự động kiểm tra trùng mã ID. |
| `remove(id)` | Xóa đối tượng theo ID: giải phóng vùng nhớ `delete *it` rồi xóa khỏi `vector`. |
| `findById(id)` | Tìm kiếm và trả về con trỏ `T*` của đối tượng có mã ID tương ứng. |
| `printById(id)` | Tìm và gọi hàm `print()` in chi tiết một đối tượng ra màn hình. |
| `printAll()` | Duyệt toàn bộ danh sách `vector<T*>` và in ra màn hình. |
| `clear()` | Duyệt giải phóng toàn bộ con trỏ trong vector để chống rò rỉ bộ nhớ (Memory Leak). |
| `loadFromFile() / saveToFile()` *(thuần ảo)* | Giao diện chuẩn buộc các Manager con phải cài đặt nạp/lưu file JSON. |

---

### 2. File `StudentManager.h` / `StudentManager.cpp`
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `loadFromFile()` | Đọc file `data/students.json` nạp danh sách sinh viên vào bộ nhớ RAM. |
| `saveToFile()` | Ghi toàn bộ danh sách sinh viên hiện tại xuống file JSON. |
| `findByClass(className)` | Lọc danh sách sinh viên theo tên lớp (ví dụ: `22T_DT1`). |
| `findByFaculty(faculty)` | Lọc danh sách sinh viên theo Khoa (ví dụ: `CNTT`). |

---

### 3. File `RoomManager.h` / `RoomManager.cpp`
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `loadFromFile()` | Đọc file `data/rooms.json`, tự động nhận diện phòng thường hoặc VIP để khởi tạo. |
| `saveToFile()` | Lưu danh sách phòng đa hình xuống file JSON. |
| `findAvailableRoom()` | Lọc ra tất cả các phòng còn chỗ trống (`!isFull()`) và không bị bảo trì. |
| `printAvailableRoom()` | In ra danh sách các phòng còn trống chỗ. |
| `findByBuilding(bld)` | Lọc danh sách phòng theo tòa nhà (Khu A, Khu VIP,...). |
| `findByType(type)` | Lọc phòng theo loại (`STANDARD` hoặc `VIP`). |
| `getOccupancyStats(...)` | Tính tổng sức chứa, tổng số người đang ở và tỷ lệ lấp đầy toàn bộ KTX. |
| `printOccupancyReport()` | In bảng báo cáo thống kê tỷ lệ lấp đầy phòng chi tiết theo bảng biểu. |
| `printRoomBill(roomId, elec, water)` | **Nghiệp vụ tính tiền:** Kích hoạt tính tiền đa hình, chia đều tiền điện nước cho sinh viên đang ở trong phòng và xuất phiếu báo chi phí. |

---

### 4. File `StaffManager.h` / `StaffManager.cpp`
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `loadFromFile() / saveToFile()` | Đọc và ghi danh sách cán bộ quản lý KTX từ file `data/staffs.json`. |
| `findByDepartment(dept)` | Lọc cán bộ theo phòng ban công tác (Ban quản lý, Kế toán,...). |

---

### 5. File `RentManager.h` / `RentManager.cpp` (Nghiệp vụ Hợp đồng KTX)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `loadFromFile(sm, rm)` | Đọc file `data/rents.json` và **đồng bộ liên kết 2 chiều** với `Student*` và `Room*`. |
| `createRent(...)` | **Đăng ký thuê phòng:** Kiểm tra phòng còn chỗ không, kiểm tra SV đã có phòng chưa $\rightarrow$ Tạo hợp đồng mới và cập nhật trạng thái phòng. |
| `terminateRent(rentId, date)` | **Trả phòng sớm:** Chuyển trạng thái hợp đồng, gỡ SV khỏi phòng và cập nhật phòng về trạng thái trống/còn chỗ. |
| `findExpiringRents(date, days)` | Lọc ra các hợp đồng sẽ hết hạn trong vòng `days` ngày tới (mặc định 30 ngày). |
| `printExpiringRents(...)` | In danh sách các hợp đồng sắp hết hạn để thông báo gia hạn. |
| `checkAndExpireRents(date)` | Tự động quét và chuyển các hợp đồng đã quá hạn sang `EXPIRED`, giải phóng phòng. |
| `calculateTotalMonthlyRevenue()` | Tính tổng doanh thu tiền phòng thu được mỗi tháng từ các hợp đồng đang `ACTIVE`. |

---

### 6. File `AccountManager.h` / `AccountManager.cpp` (Xác thực & Phân quyền)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `loadFromFile() / saveToFile()` | Đọc và lưu trữ thông tin tài khoản người dùng từ `data/accounts.json`. |
| `authenticate(username, pass)` | **Đăng nhập:** Kiểm tra tên đăng nhập, kiểm tra trạng thái khóa tài khoản và xác minh mật khẩu băm. |
| `findByUsername(username)` | Tìm kiếm thông tin tài khoản theo tên đăng nhập. |
| `findByOwnerId(ownerId)` | Tìm tài khoản liên kết với mã sinh viên hoặc mã cán bộ. |
| `createAccount(...)` | Cấp phát tài khoản mới an toàn (chống trùng username / accountId). |

---

### 7. File `DormManager.h` / `DormManager.cpp` (Bộ điều phối trung tâm Facade & Menu CLI)
| Tên hàm | Chức năng ngắn gọn |
| :--- | :--- |
| `loadAll() / saveAll()` | Nạp và lưu đồng thời toàn bộ dữ liệu của tất cả Manager. |
| `login(user, pass)` | Thực hiện đăng nhập và gán phiên làm việc vào `currentAccount`. |
| `logout()` | Hủy phiên làm việc hiện tại, đưa hệ thống về màn hình chờ. |
| `getCurrentUserRole()` | Lấy quyền hạn của người đang đăng nhập (`ADMIN`, `STAFF`, `STUDENT`). |
| `runMainMenu()` | Vòng lặp điều hướng giao diện chính, tự động phân nhánh menu theo quyền hạn. |
| `runAdminMenu()` | Giao diện điều khiển toàn quyền của Quản trị viên hệ thống. |
| `runStaffMenu()` | Giao diện làm việc của Cán bộ (quản lý phòng, sinh viên, hợp đồng, hóa đơn). |
| `runStudentMenu()` | Cổng thông tin cá nhân của Sinh viên (xem hồ sơ, xem phòng, xem hóa đơn). |
| `handleChangePassword()` | Xử lý quy trình đổi mật khẩu cá nhân an toàn có xác nhận lại. |

---

## 🚀 PHẦN 4: CHƯƠNG TRÌNH CHÍNH (`main.cpp`)

| Khối chức năng trong `main()` | Mục đích hoạt động |
| :--- | :--- |
| `SetConsoleOutputCP(CP_UTF8)` | Thiết lập bảng mã UTF-8 cho Windows Console để hiển thị tiếng Việt có dấu hoàn hảo. |
| `DormManager app;` | Khởi tạo hệ thống Mặt tiền (Facade Pattern), tự động nạp toàn bộ cơ sở dữ liệu. |
| `app.runMainMenu();` | Khởi động vòng lặp tương tác Menu Console điều hướng toàn bộ chương trình. |
