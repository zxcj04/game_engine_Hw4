#include <Ball.hpp>

int Ball::count = 0;

Ball::Ball(glm::vec3 position, float radius, glm::vec3 speed, TYPE type)
{
    this->position = position;

    BuildScene::setup_ball(vao, size);

    this->radius = radius;

    this->speed = speed;
    this->acceleration = glm::vec3(0, 0, 0);

    this->number = Ball::count++;

    this->grid_position = glm::vec3(0.0f, 0.0f, 0.0f);

    this->angle = glm::vec3(0.0f, 0.0f, 0.0f);
    this->angle_speed = glm::vec3(0.0f, 0.0f, 0.0f);
    this->angle_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    this->type = type;

    this->deleted = false;

    random_device rd;
    this->rd_generator = mt19937(rd());

    this->turn_clock = 0;

    this->movement = glm::vec3(0.0, 0.0, 0.0);

    this->last_dir = -1;
    this->turn_dir = false;

    this->state = STATE::SEARCHING;

    this->shrink_clock = 1000;
}

Ball::~Ball()
{

}

vector<bool> Ball::check_boundary_collision(float decay)
{
    vector<bool> ret(6, false);

    vector<float> boundary_value = {
         4000 - this->radius,
         -500 - this->radius,
         4000 - this->radius,
        -4000 + this->radius,
        -1000 + this->radius,
        -4000 + this->radius,
    };

    static vector<glm::vec3> boundary_normal = {
        glm::vec3(-1,  0,  0),
        glm::vec3( 0, -1,  0),
        glm::vec3( 0,  0, -1),
        glm::vec3( 1,  0,  0),
        glm::vec3( 0,  1,  0),
        glm::vec3( 0,  0,  1),
    };

    for(int i = 0 ; i < 6 ; i++)
    {
        int control = (i < 3)? 1: -1;
        bool collide = false;

        if(i == 0 || i == 3)
        {
            if(this->position.x * control >= boundary_value[i] * control)
            {
                collide = true;

                this->position.x = boundary_value[i];
            }
        }
        else if(i == 1 || i == 4)
        {
            if(this->position.y * control >= boundary_value[i] * control)
            {
                collide = true;

                this->position.y = boundary_value[i];
            }
        }
        else
        {
            if(this->position.z * control >= boundary_value[i] * control)
            {
                collide = true;

                this->position.z = boundary_value[i];
            }
        }

        if(collide)
        {
            float m1 = this->radius * this->radius * this->radius;
            float m2 = numeric_limits<float>::max();

            glm::vec3 to_wall = -boundary_normal[i];

            glm::vec3 v1_c = glm::normalize(to_wall) * glm::dot(this->speed, glm::normalize(to_wall));
            glm::vec3 v2_c = glm::vec3(0.0f, 0.0f, 0.0f);

            glm::vec3 v1_u = this->speed - v1_c;

            glm::vec3 calli_speed = ((m1 - m2) / (m1 + m2)) * v1_c + ((2 * m1) / (m1 + m2)) * v2_c;

            this->speed = v1_u + calli_speed * decay;

            ret[i] = true;

            glm::vec3 tmp = glm::cross(boundary_normal[i], this->speed);

            if(glm::length(tmp) > 0)
                tmp = glm::normalize(tmp);

            this->angle_speed += tmp * 1.0f;
        }
    }

    return ret;
}

bool Ball::check_ball_collision(Ball &ball, float decay)
{
    bool ret = false;

    if(this->type == ball.type)
        return ret;

    if(glm::distance(this->position, ball.position) <= ball.radius + this->radius)
    {
        float m1 = this->radius * this->radius * this->radius;
        float m2 = ball.radius * ball.radius * ball.radius;

        glm::vec3 to_ball = ball.position - this->position;

        glm::vec3 v1 = glm::normalize(to_ball) * glm::dot(this->speed, glm::normalize(to_ball));
        glm::vec3 v2 = glm::normalize(-to_ball) * glm::dot(ball.speed, glm::normalize(-to_ball));

        // if(this->radius < ball.radius)
            this->position = ball.position + glm::normalize(-to_ball) * (ball.radius + this->radius);
        // else
            ball.position  = this->position + glm::normalize(to_ball) * (ball.radius + this->radius);

        // this->callis.push_back();
        // ball.callis.push_back();

        this->speed = this->speed - v1 + (v1 * (m1 - m2) + 2 * m2 * v2) / (m1 + m2) * decay;
        ball.speed  = ball.speed  - v2 + (v2 * (m2 - m1) + 2 * m1 * v1) / (m1 + m2) * decay;

        ret = true;
    }

    return ret;
}

glm::vec3 get_cube_normal(glm::vec4 position)
{
    static vector<glm::vec3> to_cube_normal = {
        glm::vec3( 0,  1,  0),
        glm::vec3( 0, -1,  0),
        glm::vec3( 1,  0,  0),
        glm::vec3(-1,  0,  0),
        glm::vec3( 0,  0,  1),
        glm::vec3( 0,  0, -1),
    };

    if(position.x <= position.y && position.x >= -position.y &&
       position.z <= position.y && position.z >= -position.y)
       return to_cube_normal[0];

    if(position.x >= position.y && position.x <= -position.y &&
       position.z >= position.y && position.z <= -position.y)
       return to_cube_normal[1];

    if(position.x >= position.y && position.x >= -position.y &&
       position.x >= position.z && position.x >= -position.z)
       return to_cube_normal[2];

    if(position.x <= position.y && position.x <= -position.y &&
       position.x <= position.z && position.x <= -position.z)
       return to_cube_normal[3];

    if(position.z >= position.y && position.z >= -position.y &&
       position.x <= position.z && position.x >= -position.z)
       return to_cube_normal[4];

    if(position.z <= position.y && position.z <= -position.y &&
       position.x >= position.z && position.x <= -position.z)
       return to_cube_normal[5];

    // return glm::vec3( 0, -1,  0);
}

bool Ball::check_cube_collision(Cube &cube, float decay)
{
    bool ret = false;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-cube.rotate), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, -cube.position);

    glm::vec4 obb_position = model * glm::vec4(this->position, 1.0f);

    if(abs(obb_position.x) < cube.length / 2 + this->radius &&
       abs(obb_position.y) < cube.length / 2 + this->radius &&
       abs(obb_position.z) < cube.length / 2 + this->radius )
    {
        glm::vec4 tmp_speed = model * glm::vec4(this->speed, 0.0f);
        glm::vec3 obb_speed = glm::vec3(tmp_speed.x, tmp_speed.y, tmp_speed.z);

        float m1 = this->radius * this->radius * this->radius;
        // float m2 = cube.length * cube.length * cube.length;
        float m2 = numeric_limits<float>::max();

        glm::vec3 to_cube = cube.position - this->position;
        glm::vec3 to_cube_normal = -get_cube_normal(obb_position);

        glm::vec3 v1 = to_cube_normal * glm::dot(obb_speed, to_cube_normal);
        glm::vec3 v2 = -to_cube_normal * glm::dot(glm::vec3(0.0f, 0.0f, 0.0f), -to_cube_normal);

        glm::vec3 v1_c = to_cube_normal * glm::dot(obb_speed, to_cube_normal);
        glm::vec3 v2_c = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 v1_u = obb_speed - v1_c;

        glm::vec3 calli_speed = ((m1 - m2) / (m1 + m2)) * v1_c + ((2 * m1) / (m1 + m2)) * v2_c;

        obb_speed = v1_u + calli_speed * decay;

        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(cube.rotate), glm::vec3(0.0f, 1.0f, 0.0f));

        tmp_speed = model * glm::vec4(obb_speed, 0.0f);

        this->speed = glm::vec3(tmp_speed.x, tmp_speed.y, tmp_speed.z);

        // float len = glm::length(to_cube * to_cube_normal);

        // float ratio = ((this->radius + cube.length / 2) / len);

        glm::vec4 tmp_cube_normal = model * glm::vec4(-to_cube_normal, 0.0f);
        glm::vec3 cube_normal = glm::vec3(tmp_cube_normal.x, tmp_cube_normal.y, tmp_cube_normal.z);

        glm::vec3 tmp = glm::dot(cube_normal, to_cube) * cube_normal;

        tmp -= to_cube;

        // if(this->radius < ball.radius)
            this->position = cube.position + tmp + (this->radius + cube.length / 2) * cube_normal;
            // this->position = cube.position + -to_cube * ratio;
            // this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        // else
            // ball.position  = this->position + to_ball * (ball.radius + this->radius);

        // this->speed = this->speed - v1 + (v1 * (m1 - m2) + 2 * m2 * v2) / (m1 + m2) * decay;
        // ball.speed  = ball.speed  - v2 + (v2 * (m2 - m1) + 2 * m1 * v1) / (m1 + m2) * decay;

        ret = true;
    }

    return ret;
}

void Ball::move(vector<Ball> &balls, float decay, vector<vector<vector<glm::vec2>>> &maze_movement, vector<vector<vector<set<int>>>> &regular_grid, bool shrink)
{
    if(this->type != TYPE::FOOD && shrink_clock == 0)
    {
        if(shrink)
            this->radius -= 25.0f;

        if(this->radius < 75.0f)
        {
            this->deleted = true;

            return;
        }

        shrink_clock = 1000;
    }
    else
        shrink_clock--;

    vector<bool> boundary_collide = check_boundary_collision(decay);

    static const glm::vec3 gravity = glm::vec3(0, -3, 0);

    glm::vec3 friction = glm::vec3(0.0f, 0.0f, 0.0f);

    if(glm::length(this->speed) > 0)
        friction = glm::normalize(this->speed) * -0.15f;

    this->acceleration = glm::vec3(0, 0, 0);

    float speed = 0.5;

    if(this->type == TYPE::RANDOM)
    {
        if(turn_clock == 0)
        {
            uniform_real_distribution<float> unif(-speed, speed);
            uniform_int_distribution<int> unif_clock(500, 1000);

            this->movement.x = unif(rd_generator);
            this->movement.z = unif(rd_generator);

            turn_clock = unif_clock(rd_generator);
        }
        else
            turn_clock--;
    }
    else if(this->type == TYPE::AI)
    {
        glm::vec2 maze_position = glm::ivec2((position.x + 4000) / 500, (position.z + 4000) / 500);
        vector<glm::vec2> avail_maze = maze_movement[maze_position.x][maze_position.y];
        glm::vec3 target_position;

        this->state = STATE::SEARCHING;

        for(int i = 0 ; i < avail_maze.size() ; i++)
        {
            for(auto ball: regular_grid[avail_maze[i].x + 1][grid_position.y][avail_maze[i].y + 1])
            {
                if( state == STATE::SEARCHING && balls[ball].type == TYPE::FOOD )
                {
                    state = STATE::EAT;

                    target_position = balls[ball].position;

                    turn_clock = 0;
                }

                if( balls[ball].type == TYPE::RANDOM )
                {
                    if(this->radius > balls[ball].radius)
                    {
                        state = STATE::ATTACK;

                        target_position = balls[ball].position;

                        turn_clock = 0;
                    }
                    else
                    {
                        state = STATE::DODGE;

                        target_position = this->position * 2.0f - balls[ball].position;

                        turn_clock = 0;

                        break;
                    }
                }
            }
        }

        if(this->state == STATE::SEARCHING)
        {
            if(turn_clock == 0)
            {
                if(avail_maze.size() > 0)
                {
                    uniform_int_distribution<int> unif_target(0, avail_maze.size() - 1);

                    int target = unif_target(rd_generator);

                    target_w = avail_maze[target].x;
                    target_h = avail_maze[target].y;

                }

                target_position = glm::vec3(target_w * 500 - 4000 + 250, 0.0, target_h * 500 - 4000 + 250);

                turn_clock = 250;
            }
            else
                turn_clock--;

            // if(turn_clock == 0)
            // {
            //     uniform_real_distribution<float> unif(-speed, speed);
            //     uniform_int_distribution<int> unif_clock(500, 1000);

            //     this->movement.x = unif(rd_generator);
            //     this->movement.z = unif(rd_generator);

            //     turn_clock = unif_clock(rd_generator);
            // }
            // else
            //     turn_clock--;
        }

        this->movement = glm::normalize(target_position - position) * speed;
    }

    this->acceleration += this->movement;

    // if(boundary_collide[4] == false)
    // {
        this->acceleration += gravity;

        this->acceleration += friction;
    // }

    this->speed += this->acceleration;

    // cout << this->number << ": \t"  << this->speed.x << " \t" << this->speed.y << " \t" << this->speed.z << endl;

    this->position += this->speed;

    glm::vec3 angle_friction = glm::vec3(0.0f, 0.0f, 0.0f);

    if(glm::length(this->angle_speed) > 0.0f)
        angle_friction = glm::normalize(this->angle_speed) * -0.0035f;

    this->angle_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    // glm::vec2 tmp(this->speed.x, this->speed.z);

    // if(this->position.y <= -1000 + this->radius)
    // if(glm::length(tmp) > 1)
    // {
    //     if(glm::length(tmp) < 50)
    //         this->angle_acceleration.x += -0.005f;

    //     tmp = glm::normalize(tmp);
    //     if(this->speed.z > 0)
    //         this->angle.y = -acos(tmp.x);
    //     else
    //         this->angle.y = acos(tmp.x);

    //     // this->angle_speed.x = -0.1f;
    //     // this->angle_speed.y = 0.0f;
    // }
    // else
    // {
    //     // this->angle_speed.x = 0.0f;
    //     // this->angle.x = 0.0f;
    // }

    this->angle_acceleration += angle_friction;

    this->angle_speed += this->angle_acceleration;
    // this->angle_speed = glm::vec2(0.01f, 0.01f);

    // cout << this->number << ": \t"  << this->angle_speed.x << " \t" << this->angle_speed.y << " \t" << this->angle_speed.z << endl;

    this->angle += this->angle_speed;
}

void Ball::draw(Shader shader, unsigned int texture_ball, glm::vec3 player_position, float yaw, float pitch, float near, float far)
{
    glm::mat4 model = glm::mat4(1.0f);

    // model = glm::translate(model, player_position);
    model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0, 1, 0));
    model = glm::rotate(model, -glm::radians(pitch), glm::vec3(1, 0, 0));

    glm::mat4 model_far  = glm::scale(model, glm::vec3(far, far, far));
    glm::mat4 model_near = glm::scale(model, glm::vec3(near, near, near));

    glm::vec3 a = glm::vec3(model_far  * glm::vec4(-1,  1,  1, 1));
    glm::vec3 b = glm::vec3(model_far  * glm::vec4( 1,  1,  1, 1));
    glm::vec3 c = glm::vec3(model_far  * glm::vec4( 1, -1,  1, 1));
    glm::vec3 d = glm::vec3(model_far  * glm::vec4(-1, -1,  1, 1));

    glm::vec3 e = glm::vec3(model_near * glm::vec4(-1,  1,  1, 1));
    glm::vec3 f = glm::vec3(model_near * glm::vec4( 1,  1,  1, 1));
    glm::vec3 g = glm::vec3(model_near * glm::vec4( 1, -1,  1, 1));
    glm::vec3 h = glm::vec3(model_near * glm::vec4(-1, -1,  1, 1));

    vector<glm::vec3> planes = {
        glm::normalize(glm::cross(a, b)),
        glm::normalize(glm::cross(b, c)),
        glm::normalize(glm::cross(c, d)),
        glm::normalize(glm::cross(d, a)),
    };

    static float distance;

    BuildScene::render_ball(shader, texture_ball, vao, position, angle, radius, size, type);
}

void Ball::update_regular_grid(vector<vector<vector<set<int>>>> &regular_grid, int gap)
{
    glm::ivec3 now_grid = glm::ivec3((position + 4000.0f) / (float)gap + 1.0f);

    if(now_grid.x < 0 || now_grid.x > 8000 / gap + 1 ||
       now_grid.y < 0 || now_grid.y > 8000 / gap + 1 ||
       now_grid.z < 0 || now_grid.z > 8000 / gap + 1
    )
        return;

    if(grid_position != now_grid)
    {
        regular_grid[grid_position.x][grid_position.y][grid_position.z].erase(this->number);

        grid_position = now_grid;

        regular_grid[grid_position.x][grid_position.y][grid_position.z].insert(this->number);
    }
}