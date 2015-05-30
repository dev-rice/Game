#ifndef UnitHolder_h
#define UnitHolder_h

using namespace std;

class UnitHolder {
public:
    UnitHolder();

    void addUnit(Playable& unit);

private:
    vector<Playable> units;

};

#endif
