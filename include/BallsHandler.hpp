#pragma once

#include <iostream>
#include <vector>
#include <random>

#include <Ball.hpp>
#include <Shader.hpp>

using namespace std;

class BallsHandler
{
    public:
        BallsHandler();
        ~BallsHandler();

        void add_ball(float);
        void add_ball(float, float, float, float, float);

        void draw_balls(Shader);
        void move_balls();

        void reset_balls();

    private:
        vector<Ball> _balls;

        mt19937 rd_generator;
};