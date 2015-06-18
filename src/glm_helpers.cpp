#include "glm_helpers.hpp"

string vec3AsJsonString(glm::vec3 vector, string name) {
    // Returns a glm vec3 as a json formatted string
    // Example:
    //      "position": {
    //          "x": 0.0,
    //          "y": 0.0,
    //          "z": 0.0
    //      },
    string json_string = "\"" + name + "\": {\n";

    json_string += "\"x\": " + to_string(vector.x) + ",\n";
    json_string += "\"y\": " + to_string(vector.y) + ",\n";
    json_string += "\"z\": " + to_string(vector.z) + ",\n";

    json_string += "},\n";

    return json_string;
}
