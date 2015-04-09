#include "Updater.hpp"

namespace Core {

    Updater::Updater()
    {}

    Updater::~Updater()
    {}

    int Updater::update()
    {
        return 0;
    }

    int Updater::entry_point(void* me)
    {
        return static_cast<Updater*>(me)->update();
    }

}
