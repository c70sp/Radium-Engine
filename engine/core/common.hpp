// C++ STL
#include <fstream>
#include <string>
#include <glm/glm.hpp>

std::string loadFileAsString(const std::string& path);
void checkGLError(const std::string& location);
glm::vec3 worldToScreen(const glm::vec3& position, const glm::mat4& VPmat, int screenWidth, int screenHeight);