// map.h
// Trevor Westphal

class Map {
public:
    Map(const char *, const char**);
private:
    void generateMesh();
    void draw();
};