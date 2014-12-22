#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "camera.h"
#include "texture_set.h"
#include "text_renderer.h"

class DebugInfo {
public:
    DebugInfo(GLFWwindow*);

    void startFrame();
    void endFrame();

    void draw();

private:
    float start_time;
    float current_fps;

    TextRenderer* text;
};