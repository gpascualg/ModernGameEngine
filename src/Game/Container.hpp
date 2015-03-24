#pragma once

#include "core.hpp"
#include <vector>

class Object;

class Container
{
public:
    Container();

    void addChild(Object* object);
    Object* getChild(uint32_t idx);
    Object* getChildByID(uint32_t id);

private:
    std::vector<Object*> _children;
};
