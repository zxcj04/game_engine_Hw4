#pragma once

#include <iostream>
#include <limits>
#include <map>
#include <vector>
#include <set>

#include <glm/glm.hpp>

#include <constants.hpp>
#include <BuildScene.hpp>
#include <Shader.hpp>

using namespace std;

class Ball
{
    public:
        Ball(glm::vec3, float, glm::vec3);
        ~Ball();

        void draw(Shader, glm::vec3, float, float, float, float);
        void move(vector<Ball>&, bool, float);

        bool check_boundary_collision(vector<glm::vec3>&, float);
        bool check_ball_collision(Ball &, float);

        void update_regular_grid(vector<vector<vector<set<int>>>> &regular_grid, int);

        static int count;

    private:

        glm::vec3 position;
        glm::vec3 speed;
        glm::vec3 acceleration;

        glm::ivec3 grid_position;

        int number;

        unsigned int vao;
        float radius;
        int size;

        vector<glm::vec3> callis;

};