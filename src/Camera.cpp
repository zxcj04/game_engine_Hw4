#include <./Camera.hpp>

Camera::Camera()
{
    this->zoom_value = 30.0f;

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);

    this->yaw        = 225.0f;
    this->pitch      = 0.0f;

    this->left   =  -1500 ;
    this->right  =   1500 ;
    this->bottom =  -1500 ;
    this->top    =   1500 ;
    this->near   =  -1500 ;
    this->far    =   1500 ;
}

Camera::Camera(float boundary_size)
{
    this->zoom_value = 30.0f;

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);

    this->yaw        = 225.0f;
    this->pitch      = 0.0f;

    this->left   =  -(boundary_size * 3/4) ;
    this->right  =   (boundary_size * 3/4) ;
    this->bottom =  -(boundary_size * 3/4) ;
    this->top    =   (boundary_size * 3/4) ;
    this->near   =  -(boundary_size * 3/4) ;
    this->far    =   (boundary_size * 3/4) ;
}

void Camera::update_boundary_size(float boundary_size)
{
    this->left   =  -(boundary_size * 3/4) ;
    this->right  =   (boundary_size * 3/4) ;
    this->bottom =  -(boundary_size * 3/4) ;
    this->top    =   (boundary_size * 3/4) ;
    this->near   =  -(boundary_size * 3/4) ;
    this->far    =   (boundary_size * 3/4) ;

    float ratio = 30.0f;

    ratio /= zoom_value;

    this->left /= ratio;
    this->right /= ratio;
    this->bottom /= ratio;
    this->top /= ratio;
}

void Camera::calc(SCENE scene)
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 view_up;

    // cout << "\t" << direction.x << " " << direction.y << " " << direction.z << endl;

    switch(scene)
    {
        case SCENE::FIRST:

            direction.x = -sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            direction.y = sin(glm::radians(this->pitch));
            direction.z = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            direction = glm::normalize(direction);

            this->direction = direction;

            position = this->position;

            view_up = glm::vec3(0, 1, 0);

            break;

        case SCENE::ORTHO_X:

            direction = glm::vec3(1, 0, 0);
            direction = glm::normalize(direction);

            position = glm::vec3(0, 0, 0);

            view_up = glm::vec3(0, 1, 0);

            break;

        case SCENE::ORTHO_Y:

            direction = glm::vec3(0, -1, 0);
            direction = glm::normalize(direction);

            position = glm::vec3(0, 0, 0);

            view_up = glm::vec3(1, 0, 0);

            break;

        case SCENE::ORTHO_Z:

            direction = glm::vec3(0, 0, 1);
            direction = glm::normalize(direction);

            position = glm::vec3(0, 0, 0);

            view_up = glm::vec3(0, 1, 0);

            break;
    }

    // cout << "\t" << direction.x << " " << direction.y << " " << direction.z << endl;

    glm::vec3 view_right = glm::normalize(glm::cross(direction, view_up)); // member consider
    glm::vec3 up = glm::normalize(glm::cross(view_right, direction));

    view = glm::lookAt(position, position + direction, up);
}

void Camera::use(Shader shader, SCENE scene)
{
    calc(scene);

    // cout << "camera: " << endl;

    // cout << position.x << " " << position.y << " " << position.z << endl;

    // cout << direction.x << " " << direction.y << " " << direction.z << endl;

    // cout << yaw << " " << pitch << endl;

    shader.set_uniform("view", view);
    shader.set_uniform("view_pos", this->position);
    shader.set_uniform("light_pos", -this->position);
    shader.set_uniform("eye_direction", this->direction);
}

void Camera::zoom(float offset)
{
    static float sensitivity = 1.0f;

    float ratio = zoom_value;

    zoom_value -= offset * sensitivity;

    if(zoom_value < 1)
        zoom_value = 1;

    ratio /= zoom_value;

    this->left /= ratio;
    this->right /= ratio;
    this->bottom /= ratio;
    this->top /= ratio;
}

void Camera::update_yaw(float offset)
{
    static float sensitivity = 0.2f;

    yaw += offset * sensitivity;

    if(yaw > 360.0f)
        yaw -=  360.0f;
    if(yaw <   0.0f)
        yaw +=  360.0f;
}

void Camera::update_pitch(float offset)
{
    static float sensitivity = 0.1f;

    pitch += offset * sensitivity;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
}