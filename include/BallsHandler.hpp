#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <set>

#include <Ball.hpp>
#include <Shader.hpp>

using namespace std;

class BallsHandler
{
    public:
        BallsHandler();
        ~BallsHandler();

        void add_ball(float);
        void add_ball(float, glm::vec3, float, glm::vec3);

        void draw_balls(Shader, glm::vec3, float, float, float, float);
        void move_balls(bool, float, bool);

        void reset_balls();

    private:
        vector<Ball> _balls;

        mt19937 rd_generator;

        vector<vector<vector<set<int>>>> regular_grid;

        int gap;
};