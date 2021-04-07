#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <constants.hpp>
#include <Shader.hpp>

class Camera
{
    public:

        glm::vec3 position;

        float yaw;
        float pitch;

        Camera();

        void calc(SCENE);
        void use(Shader, SCENE);

        void zoom(float);

        void update_yaw(float);
        void update_pitch(float);

        float left, right, top, bottom, near, far;

    // private:
        glm::vec3 direction;

        glm::mat4 view;

        float zoom_value;
};