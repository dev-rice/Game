#ifndef UnitHolder_h
#define UnitHolder_h

#include "playable.hpp"

using namespace std;

class UnitHolder {
public:
    UnitHolder();

    void addUnit(Playable& unit);

    vector<Playable>& getUnits();

private:
    vector<Playable> units;

};

#endif
