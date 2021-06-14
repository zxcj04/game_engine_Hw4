#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <set>

#include <Ball.hpp>
#include <Cube.hpp>
#include <Shader.hpp>

#include <constants.hpp>

using namespace std;

class BallsHandler
{
    public:
        BallsHandler(float boundary_size);
        ~BallsHandler();

        void add_ball(float);
        void add_ball(glm::vec3, float, glm::vec3, TYPE);

        void add_cube(float, float x, float y);

        void draw_balls(Shader, unsigned int, glm::vec3, float, float, float, float);
        void move_balls(float, bool);

        void draw_cubes(Shader, unsigned int, bool);

        void reset_balls();
        void reset_cubes(float);

    private:
        vector<Ball> _balls;
        vector<Cube> _cubes;

        mt19937 rd_generator;

        vector<vector<vector<set<int>>>> regular_grid;

        vector<vector<vector<set<int>>>> cube_regular_grid;

        vector<vector<int>> maze;

        int gap;
};