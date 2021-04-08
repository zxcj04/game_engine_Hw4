#include <Ball.hpp>

Ball::Ball(float x, float y, float z, float radius)
{
    position = glm::vec3(x, y, z);

    BuildScene::setup_ball(vao, size);

    this->ball_radius = radius;
}

Ball::~Ball()
{

}

void Ball::move()
{

}

void Ball::draw(Shader shader)
{
    BuildScene::render_ball(shader, vao, position, ball_radius, size);
}