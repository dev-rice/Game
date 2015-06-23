#ifndef BillboardDrawable_h
#define BillboardDrawable_h

#include <string>

#include "drawable.h"
#include "resource_loader.hpp"
#include "plane_mesh.hpp"

using namespace std;

class BillboardDrawable : public Drawable {
public:
    BillboardDrawable(ResourceLoader& resource_loader);
    ~BillboardDrawable();

    Drawable* clone();

    void draw();

    string asJsonString();
private:
    float opacity;
    float plane_rotation;
    void updateUniformData();

};

#endif
