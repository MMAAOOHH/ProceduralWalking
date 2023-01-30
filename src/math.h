#pragma once
#include <algorithm>

inline GLfloat calculate_distance(const glm::vec2& p1, const glm::vec2& p2)
{
    GLfloat dif_y = p1.y - p2.y;
    GLfloat dif_x = p1.x - p2.x;
    return sqrt((dif_y * dif_y) + (dif_x * dif_x));
}

inline GLfloat calculate_angle(const glm::vec2& v1, const glm::vec2& v2)
{
    if (glm::length(v1) == 0 || glm::length(v2) == 0)
        return 0;

    glm::vec2 const v1_norm = glm::normalize(v1);
    glm::vec2 const v2_norm = glm::normalize(v2);

    GLfloat const dot = glm::dot(v1_norm, v2_norm);
    GLfloat const magnitude = glm::length(v1) * glm::length(v2);
    GLfloat const angle = acos(dot / magnitude);
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


inline glm::vec2 slerp(glm::vec2 start, glm::vec2 end, GLfloat t)
{
    GLfloat angle = acos(glm::dot(start, end));
    return (start * sin((1 - t) * angle) + end * sin(t * angle)) / sin(angle);
}

inline glm::vec2 rotate(const glm::vec2 start, const glm::vec2 pivot, float angle) {
    float s = sin(angle);
    float c = cos(angle);

    glm::vec2 result;
    result.x = (start.x - pivot.x) * c - (start.y - pivot.y) * s + pivot.x;
    result.y = (start.x - pivot.x) * s + (start.y - pivot.y) * c + pivot.y;
    return result;
}

inline glm::vec2 rotate180(const glm::vec2 start, const glm::vec2 pivot) {
    return rotate(start, pivot, glm::pi<GLfloat>());
}

/*
inline glm::vec2 slerp(glm::vec2 a, glm::vec2 b, GLfloat t) {

    GLfloat dot = glm::dot(a, b);
    dot = glm::clamp(dot, -1.0f, 1.0f);
    GLfloat theta = std::acos(dot) * t;
    glm::vec2 direction = b - a * t;
    direction = normalize(direction);
    glm::vec2 result = (a * std::cos(theta)) + (direction * std::sin(theta));
    return result;
}
*/