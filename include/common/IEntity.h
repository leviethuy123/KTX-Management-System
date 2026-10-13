#ifndef IENTITY_H
#define IENTITY_H

#include <string>


class IEntity {
public:
    virtual ~IEntity() = default;

    virtual std::string getId() const = 0;
    virtual void print() const = 0;
    virtual std::string serialize() const = 0;
};

#endif 
