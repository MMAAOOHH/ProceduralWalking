#pragma once

#include "rect.h"
#include "renderer.h"

struct GameObject
{
    std::vector<std::shared_ptr<drawable>> drawables;

    virtual void start(){}
    virtual void update(GLfloat dt)
    {
    }
    virtual void draw(SpriteRenderer& renderer, const glm::mat4& view)
    {
        if (drawables.empty()) return;
        for (auto& drawable : drawables)
            renderer.draw(*drawable, view);
    }
};
