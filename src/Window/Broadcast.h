#pragma once

#include "core.h"
#include "counter.h"

#include <functional>
#include <map>
#include <vector>

class CleanBroadcast
{
public:
    virtual ~CleanBroadcast()
    {}

    static void unbindAll()
    {
        while (!broadcasts.empty())
        {
            delete broadcasts.back();
            broadcasts.pop_back();
        }
    }

protected:
    CleanBroadcast()
    {}

protected:
    static std::vector<CleanBroadcast*> broadcasts;
};

template<typename ... Parameters>
class Broadcast_base
{
    template <typename ... T>
    friend class Broadcast;

protected:
    Broadcast_base(uintptr_t uniqueID):
        _uniqueID(uniqueID)
    {}

    virtual ~Broadcast_base()
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
class Broadcast_imp : public Broadcast_base<Parameters...>, public CleanBroadcast
{
    using Signal = void (Class::*)(Parameters...);
    using Slot = void (Callee::*)(Parameters...);

    template <typename ... T>
    friend class Broadcast;

private:
    Broadcast_imp(uintptr_t uniqueID):
        Broadcast_base<Parameters...>(uniqueID)
    {
        broadcasts.push_back(this);
    }

    virtual ~Broadcast_imp()
    {}

    void bind(Class* cl, Signal method,
        Callee* callee, Slot callback)
    {
        uintptr_t id = (uintptr_t)(void*&)method;
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
    ~Broadcast()
    {
        auto it = implementations.begin();
        for (; it != implementations.end(); ++it)
        {
            delete it->second;
        }
    }

    template <class Class, class Callee>
    static void bind(Class* cl, void (Class::*method)(Parameters...),
        Callee* callee, void (Callee::*callback)(Parameters...))
    {
        using Implementation = Broadcast_imp<Class, Callee, Parameters...>;
        uintptr_t id = (uintptr_t)cl;
        Implementation* broadcast = nullptr;

        auto it = implementations.begin();
        if ((it = implementations.find(id)) != implementations.end())
        {
            broadcast = (Implementation*)it->second;
        }
        else
        {
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


template <class Class, class Callee, typename ... Parameters>
void bind(Class* cl, void (Class::*method)(Parameters...),
    Callee* callee, void (Callee::*callback)(Parameters...))
{
    Broadcast<Parameters...>::bind(cl, method, callee, callback);
}

template <class Class, typename ... Parameters>
void emit(Class* cl, void (Class::*method)(Parameters...),
    Parameters ... args)
{
    Broadcast<Parameters...>::emit(cl, method, args...);
}

inline void unbindAll()
{
    CleanBroadcast::unbindAll();
}
