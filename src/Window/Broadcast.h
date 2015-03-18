#pragma once

#include "core.h"
#include "counter.h"

#include <functional>
#include <map>
#include <vector>

template<typename ... Parameters>
class Broadcast_base
{
public:
    Broadcast_base(uintptr_t uniqueID):
        _uniqueID(uniqueID)
    {}

    bool is(uintptr_t uniqueID)
    {
        return _uniqueID == uniqueID;
    }

    virtual void call(uintptr_t sid, Parameters ... args) = 0;

private:
    uintptr_t _uniqueID;
};

template <class Class, class Callee, typename ... Parameters>
class Broadcast_imp : public Broadcast_base<Parameters...>
{
    using Signal = void (Class::*)(Parameters...);
    using Slot = void (Callee::*)(Parameters...);

public:
    Broadcast_imp(uintptr_t uniqueID):
        Broadcast_base<Parameters...>(uniqueID)
    {}

    void bind(Class* cl, Signal method,
        Callee* callee, Slot callback)
    {
        uintptr_t id = (uintptr_t)(void*&)method;
        printf("[BIND] Binding new Method(%lu)\n", id);
        callbacks[id].push_back(std::make_pair(callee, callback));
    }

    void call(uintptr_t sid, Parameters ... args)
    {
        auto it = callbacks[sid].begin();

        for (; it != callbacks[sid].end(); ++it)
        {
            Callee* cl = it->first;
            Slot callback = it->second;

            (cl->*callback)(args...);
        }
    }

private:
    std::map<uintptr_t, std::vector<std::pair<Callee*, Slot> > > callbacks;
};

template <typename ... Parameters>
class Broadcast
{
public:
    template <class Class, class Callee>
    static void bind(Class* cl, void (Class::*method)(Parameters...),
        Callee* callee, void (Callee::*callback)(Parameters...))
    {
        using Implementation = Broadcast_imp<Class, Callee, Parameters...>;
        uintptr_t id = (uintptr_t)cl;
        Implementation* broadcast = NULL;

        auto it = implementations.begin();
        if ((it = implementations.find(id)) != implementations.end())
        {
            broadcast = (Implementation*)it->second;
        }
        else
        {
            printf("[BIND] Creating new Implementation(%lu)\n", id);
            broadcast = new Implementation(id);
            implementations.insert(std::make_pair(id, broadcast));
        }

        broadcast->bind(cl, method, callee, callback);
    }

    template <class Class>
    static void emit(Class* cl, void (Class::*method)(Parameters...),
        Parameters ... args)
    {
        uintptr_t id = (uintptr_t)cl;
        uintptr_t sid = (uintptr_t)(void*&)method;

        printf("[EMIT] Calling %lu(%lu))\n", id, sid);

        auto it = implementations.begin();
        if ((it = implementations.find(id)) != implementations.end())
        {
            Broadcast_base<Parameters...>* broadcast = it->second;
            if (broadcast->is(id))
            {
                broadcast->call(sid, args...);
            }
        }
    }

private:
    static std::map<uintptr_t, Broadcast_base<Parameters...>*> implementations;
};


template<typename ... Parameters>
std::map<uintptr_t, Broadcast_base<Parameters...>*> Broadcast<Parameters...>::implementations;
