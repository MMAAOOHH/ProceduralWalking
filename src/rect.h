#pragma once

#include <iostream>
#include <glad/glad.h>
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "material.h"
#include "math.h"

struct Drawable
{
    enum transform_origin
    {
	    centered,
        center_left,
        bottom_middle,
    };

    transform_origin transform_origin = centered;
    glm::vec2 position = glm::vec2(0.0f);
    GLfloat rotation = 0.0f;
    glm::vec2 size = glm::vec2(64.0f);

    Material* material;

    glm::mat4 mat4_translated(glm::mat4 matrix) const
    {
        glm::vec2 normal_offset = glm::vec2(0.0f);

        switch (transform_origin)
        {
        case centered:
            normal_offset.x = 0.5f;
            normal_offset.y = 0.5f;
            break;
        case center_left:
            normal_offset.y = 0.5f;
            break;
        case bottom_middle:
            normal_offset.x = 0.5f;
            normal_offset.y = 1.0f;
            break;
        }
        return glm::translate(matrix, glm::vec3(-normal_offset * size, 0.0f));
    }

    glm::mat4 get_model_transform() const
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = mat4_translated(model);
    	model = glm::scale(model, glm::vec3(size, 1.0f));

        return model;
    }
};


struct Line : Drawable
{
    glm::vec2 start;
    glm::vec2 end;
    GLfloat thickness = 25.0f;

    Line()
    {
        transform_origin = center_left;
        position = start;
        size.x = calculate_distance(start, end); // b - a, distance
        rotation = atan2(end.y, end.x) - atan2(start.y, start.x);
        size.y = thickness;
    }

	void update(GLfloat dt) 
    {
        start = position;
        size.y = thickness;
        size.x = calculate_distance(start, end);
        GLfloat angle = atan2(start.y, start.x) - atan2(end.y, end.x);
        rotation = angle;
    }
};

struct rect
{
    GLfloat x, y, w, h;
};
