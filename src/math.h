#pragma once

inline GLfloat calculate_distance(const glm::vec2& p1, const glm::vec2& p2)
{
    GLfloat dif_y = p1.y - p2.y;
    GLfloat dif_x = p1.x - p2.x;
    return sqrt((dif_y * dif_y) + (dif_x * dif_x));
}

inline GLfloat calculate_angle(const glm::vec2& v1, const glm::vec2 v2)
{
    GLfloat dot = glm::dot(glm::normalize(v1), glm::normalize(v1));
    GLfloat magnitude = glm::length(v1) * glm::length(v2);
    GLfloat angle = acos(dot / magnitude);
    return angle;
}

inline glm::vec2 lerp(glm::vec2 a, glm::vec2 b, GLfloat t)
{
    return a + t * (b - a);
}

inline GLfloat lerp(GLfloat a, GLfloat b, GLfloat t)
{
    return a + t * (b - a);
}

inline glm::vec2 lerp(glm::vec3 a, glm::vec3 b, GLfloat t)
{
    return a + t * (b - a);
}