#include "Intersect.h"

intersection checkIntersect(float xpos, float ypos, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    glm::vec3 p(xpos, ypos, 0.0f);

    glm::vec3 a(v1.x, v1.y, 0.0f);
    glm::vec3 b(v2.x, v2.y, 0.0f);
    glm::vec3 c(v3.x, v3.y, 0.0f);

    float c1 = glm::cross(b - a, p - a).z;
    float c2 = glm::cross(c - b, p - b).z;
    float c3 = glm::cross(a - c, p - c).z;

    intersection intersect;
    intersect.hit = ((c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0));
    if (intersect.hit) {
        float totalArea = c1 + c2 + c3;
        intersect.bery.x = c1 / totalArea;
        intersect.bery.y = c2 / totalArea;
        intersect.bery.z = c3 / totalArea;

        intersect.Z = intersect.bery.x * v1.z + intersect.bery.y * v2.z + intersect.bery.z * v3.z;
    }

    return intersect;
}
