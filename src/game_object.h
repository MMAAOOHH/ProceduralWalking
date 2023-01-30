#pragma once

#include "rect.h"
#include "renderer.h"

struct Transform
{
    glm::vec2 position = { 0,0 };
    float rotation = 0.0f;
    float scale = 1.0f;
};

struct GameObject
{
    Transform transform;
    Drawable drawable;

    virtual void start() {}
    virtual void update(GLfloat dt)
    {
            drawable.position = transform.position;
            drawable.rotation = transform.rotation;
    }
    virtual void draw(SpriteRenderer& renderer, const glm::mat4& view)
    {
            renderer.draw(drawable, view);
    }
};