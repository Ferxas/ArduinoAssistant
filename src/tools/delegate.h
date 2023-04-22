#include <vector>
#include <algorithm>

struct Delegate {
    std::vector<void(*)()> funcs;

    template<class T> void operator+=(T mFunc)
    {
        funcs.push_back(mFunc);
    }

    template<class T> void operator-=(T mFunc)
    {
        funcs.erase(std::remove(funcs.begin(), funcs.end(), mFunc), funcs.end());
    }

    void operator()() {
        for (auto& f : funcs) f();
    }
};