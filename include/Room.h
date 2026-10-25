#ifndef ROOM_H
#define ROOM_H

#include "Common.h"
#include <vector>
#include <string>

class Rent; // Forward declaration tránh phụ thuộc vòng

// =========================================================================
// 1. LỚP THIẾT BỊ HỎNG TRONG PHÒNG (BROKENDEVICE)
// =========================================================================

class BrokenDevice {
private:
    std::string deviceId;      // Mã định danh thiết bị hỏng (ví dụ: TB01)
    std::string deviceName;    // Tên thiết bị (ví dụ: Quạt trần, Bàn học, Bóng đèn, Điều hòa,...)
    std::string description;   // Mô tả hư hỏng / tình trạng (ví dụ: Gãy chân, Không quay, Chập cháy)
    Date reportDate;           // Ngày ghi nhận báo hỏng
    bool isRepaired;           // Trạng thái: false = Đang hỏng, true = Đã sửa chữa

public:
    BrokenDevice();
    BrokenDevice(const std::string& id, const std::string& name,
                 const std::string& desc, const Date& date = Date(), bool repaired = false);

    std::string getDeviceId() const;
    std::string getDeviceName() const;
    std::string getDescription() const;
    Date getReportDate() const;
    bool isFixed() const;

    void setDeviceId(const std::string& id);
    void setDeviceName(const std::string& name);
    void setDescription(const std::string& desc);
    void setReportDate(const Date& date);
    void setFixed(bool fixed);

    void print() const;
    nlohmann::json toJson() const;
    static BrokenDevice fromJson(const nlohmann::json& j);
};

// =========================================================================
// 2. LỚP CƠ SỞ TRỪU TƯỢNG PHÒNG (ROOM)
// =========================================================================

class Room : public IEntity {
protected:
    std::string roomId;                     // Mã phòng: P101, P201
    std::string building;                   // Tòa nhà: Khu A, Khu VIP
    int floor;                              // Tầng
    int maxStudents;                        // Sức chứa tối đa
    double basePrice;                       // Giá thuê cơ bản (Đưa lên lớp cha vì phòng nào cũng có giá)
    RoomStatus status;                      // Trạng thái: EMPTY, OCCUPIED, FULL, MAINTENANCE
    std::vector<Rent*> activeRent;          // Hợp đồng của sinh viên đang ở
    std::vector<BrokenDevice> brokenDevices;// Danh sách các thiết bị hỏng trong phòng

public:
    Room();
    Room(const std::string& id, int maxStd, int fl, const std::string& bld,
         double price = 0.0, RoomStatus st = RoomStatus::EMPTY);
    virtual ~Room();

    std::string getRoomId() const;
    int getMaxStudents() const;
    RoomStatus getStatus() const;
    int getFloor() const;
    std::string getBuilding() const;
    double getBasePrice() const;
    const std::vector<Rent*>& getActiveRent() const;

    // Danh mục trang thiết bị (Tính Đa hình Polymorphism)
    virtual std::string getEquipmentList() const = 0;
    virtual std::string getAmenities() const;
    bool hasEquipment(const std::string& deviceName) const;
    bool isEquipmentWorking(const std::string& deviceName) const;

    // Quản lý thiết bị hỏng
    const std::vector<BrokenDevice>& getBrokenDevices() const;
    void addBrokenDevice(const BrokenDevice& dev);
    bool removeBrokenDevice(const std::string& deviceId);
    bool fixBrokenDevice(const std::string& deviceId);
    int countUnfixedDevices() const;
    bool hasBrokenDevices() const;
    void printBrokenDevices() const;

    void setRoomId(const std::string& id);
    void setMaxStudents(int maxStd);
    void setStatus(RoomStatus st);
    void setFloor(int fl);
    void setBuilding(const std::string& bld);
    void setBasePrice(double price);

    void addRent(Rent* rent);
    bool removeRent(Rent* rent);
    bool isEmpty() const;
    bool isFull() const;

    virtual std::string getRoomType() const = 0;
    virtual double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const = 0;

    std::string getId() const override;
    void print() const override;
    virtual nlohmann::json toJson() const override = 0;
    std::string serialize() const override {
        return toJson().dump(2);
    }
};

// =========================================================================
// 3. LỚP PHÒNG TIÊU CHUẨN (STANDARDROOM - KẾ THỪA ROOM)
// =========================================================================

class StandardRoom : public Room {
public:
    StandardRoom();
    StandardRoom(const std::string& rId, int fl, const std::string& bld,
                 double price = 400000.0, RoomStatus st = RoomStatus::EMPTY);
    virtual ~StandardRoom() = default;

    std::string getRoomType() const override;
    std::string getEquipmentList() const override;
    double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const override;

    nlohmann::json toJson() const override;
    std::string serialize() const override;
    static StandardRoom fromJson(const nlohmann::json& j);
    static StandardRoom deserialize(const std::string& jsonStr);
};

// =========================================================================
// 4. LỚP PHÒNG VIP DỊCH VỤ CAO CẤP (VIPROOM - KẾ THỪA ROOM)
// =========================================================================

class VipRoom : public Room {
private:
    double serviceFee; // Phí dịch vụ VIP riêng biệt (ví dụ: 250,000 VNĐ)

public:
    VipRoom();
    VipRoom(const std::string& rId, int fl, const std::string& bld,
            double price = 1200000.0, double sFee = 250000.0, RoomStatus st = RoomStatus::EMPTY);
    virtual ~VipRoom() = default;

    double getServiceFee() const;
    void setServiceFee(double sFee);

    std::string getRoomType() const override;
    std::string getEquipmentList() const override;
    double calculateTotalFee(double electricUsage = 0, double waterUsage = 0) const override;

    nlohmann::json toJson() const override;
    std::string serialize() const override;
    static VipRoom fromJson(const nlohmann::json& j);
    static VipRoom deserialize(const std::string& jsonStr);
};

#endif // ROOM_H
