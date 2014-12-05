// level_loader.h
// Trevor Westphal

#include <vector>

#include "drawable.h"

class Level{
public:
    Level();
    void loadLevel(const char *);

    std::vector<Drawable*> getDrawable();
private:
    
};