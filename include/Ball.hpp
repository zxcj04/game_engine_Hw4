#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include <BuildScene.hpp>
#include <Shader.hpp>

using namespace std;

class Ball
{
    public:
        Ball(float, float, float, float);
        ~Ball();

        void draw(Shader);
        void move();

    private:

        glm::vec3 position;

        unsigned int vao;
        float ball_radius;
        int size;

};