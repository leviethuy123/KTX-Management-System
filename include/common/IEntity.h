#ifndef IENTITY_H
#define IENTITY_H

#include <string>
#include <nlohmann/json.hpp>

/**
 * @brief Interface cơ sở cho tất cả thực thể trong hệ thống KTX.
 * Bắt buộc mọi thực thể phải có:
 * - getId(): Dùng để tìm kiếm và định danh trong Manager<T>
 * - print(): In thông tin định dạng đẹp ra màn hình Console
 * - toJson(): Xuất đối tượng thành cấu trúc JSON (thay thế cho file .dat)
 * - serialize(): Xuất JSON dạng chuỗi có định dạng (indent 2)
 */
class IEntity {
public:
    virtual ~IEntity() = default;

    virtual std::string getId() const = 0;
    virtual void print() const = 0;
    virtual nlohmann::json toJson() const = 0;

    virtual std::string serialize() const {
        return toJson().dump(2);
    }
};

#endif // IENTITY_H
