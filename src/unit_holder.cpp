#include "unit_holder.hpp"

UnitHolder::UnitHolder(){

}

void UnitHolder::addUnit(Playable& unit){
    units.push_back(unit);
}

vector<Playable>& UnitHolder::getUnits(){
    return units;
}
