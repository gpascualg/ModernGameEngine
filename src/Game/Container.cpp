#include "Container.hpp"
#include "Object.hpp"

#include <cassert>

Container::Container()
{

}

void Container::addChild(Object* object)
{
    _children.push_back(object);
}

Object* Container::getChild(uint32_t idx)
{
    assert(idx < _children.size() && "Invalid index");

    return _children[idx];
}

Object* Container::getChildByID(uint32_t id)
{
    for (auto it = _children.begin(); it != _children.end(); ++it)
    {
        if ((*it)->getID() == id)
        {
            return *it;
        }
    }

    return NULL;
}
