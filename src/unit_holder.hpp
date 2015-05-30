#ifndef UnitHolder_h
#define UnitHolder_h

#include "playable.h"

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
