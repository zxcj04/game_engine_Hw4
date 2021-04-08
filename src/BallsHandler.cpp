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
    uniform_real_distribution<float> unif_radius(1, 5);

    add_ball(boundary_size
           , unif_position(rd_generator), unif_position(rd_generator), unif_position(rd_generator)
           , unif_radius(rd_generator));
}

void BallsHandler::draw_balls(Shader shader)
{
    for(auto & ball: _balls)
        ball.draw(shader);
}

void BallsHandler::move_balls()
{
    for(auto & ball: _balls)
        ball.move();
}

void BallsHandler::reset_balls()
{
    _balls.clear();
}