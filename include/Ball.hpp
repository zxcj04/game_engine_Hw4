#pragma once

#include <iostream>
#include <limits>
#include <map>
#include <vector>
#include <random>
#include <set>

#include <glm/glm.hpp>

#include <constants.hpp>
#include <BuildScene.hpp>
#include <Shader.hpp>
#include <Cube.hpp>

using namespace std;

enum STATE
{
    SEARCHING, EAT, ATTACK, DODGE
};

class Ball
{
    public:
        Ball(glm::vec3, float, glm::vec3, TYPE);
        ~Ball();

        void draw(Shader, unsigned int, glm::vec3, float, float, float, float);
        void move(vector<Ball>&, float, vector<vector<vector<glm::vec2>>> &maze_movement, vector<vector<vector<set<int>>>> &regular_grid, bool shrink);

        vector<bool> check_boundary_collision(float);
        bool check_ball_collision(Ball &, float);
        bool check_cube_collision(Cube &, float);

        void update_regular_grid(vector<vector<vector<set<int>>>> &regular_grid, int);

        static int count;

    // private:

        glm::vec3 position;
        glm::vec3 speed;
        glm::vec3 acceleration;

        glm::vec3 angle;
        glm::vec3 angle_speed;
        glm::vec3 angle_acceleration;

        glm::ivec3 grid_position;

        int number;

        unsigned int vao;
        float radius;
        int size;

        TYPE type;

        int shrink_clock;

        bool deleted;

        mt19937 rd_generator;

        int turn_clock;
        glm::vec3 movement;

        int last_dir;
        bool turn_dir;
        int target_w, target_h;

        STATE state;
};