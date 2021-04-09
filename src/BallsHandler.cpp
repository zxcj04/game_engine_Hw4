#include <BallsHandler.hpp>

BallsHandler::BallsHandler()
{
    random_device rd;

    rd_generator = mt19937(rd());
}

BallsHandler::~BallsHandler()
{

}

void BallsHandler::add_ball(float boundary_size, float x, float y, float z, float radius)
{
    _balls.push_back(Ball(x, y, z, radius));
}

void BallsHandler::add_ball(float boundary_size)
{
    uniform_real_distribution<float> unif_position(-boundary_size / 2, boundary_size / 2);
    uniform_real_distribution<float> unif_radius(50, 250);

    float radius = unif_radius(rd_generator);

    float x, y, z;

    do
    {
        x = unif_position(rd_generator);
        y = unif_position(rd_generator);
        z = unif_position(rd_generator);

    } while (abs(x) + radius >= 1000 ||
             abs(y) + radius >= 1000 ||
             abs(z) + radius >= 1000);


    add_ball(boundary_size, x, y, z, radius);
}

void BallsHandler::draw_balls(Shader shader, glm::vec3 player_position, float yaw, float pitch, float near, float far)
{
    for(auto & ball: _balls)
        ball.draw(shader, player_position, yaw, pitch, near, far);
}

void BallsHandler::move_balls(bool cool)
{
    for(int i = 0; i < _balls.size(); i++)
        for(int j = i + 1; j < _balls.size() ; j++)
        {
            _balls[i].check_ball_collision(_balls[j]);
        }

    for(auto & ball: _balls)
        ball.move(_balls, cool);
}

void BallsHandler::reset_balls()
{
    _balls.clear();

    Ball::count = 0;
}