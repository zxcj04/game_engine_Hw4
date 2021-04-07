#pragma once

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <constants.hpp>
#include <Camera.hpp>
#include <Shader.hpp>

using namespace std;

class BuildScene
{
    public:
        static void set_viewport(SCENE, int, int);
        static void set_projection(SCENE, glm::mat4 &, Camera, int, int);

        static void setup_boundary(unsigned int &);
        static void render_boundary(SCENE, unsigned int, Shader, unsigned int);

        static void setup_player(unsigned int &);
        static void render_player(unsigned int, Shader, glm::vec3);

        static void setup_texture(unsigned int &, string);
};