#include <Ball.hpp>

int Ball::count = 0;

Ball::Ball(float x, float y, float z, float radius)
{
    position = glm::vec3(x, y, z);

    BuildScene::setup_ball(vao, size);

    this->radius = radius;

    this->speed = glm::vec3(0, -10, 0);
    this->acceleration = glm::vec3(0, -3, 0);

    this->number = ++Ball::count;
}

Ball::~Ball()
{

}

bool Ball::check_boundary_collision(vector<glm::vec3> &callis)
{
    bool ret = false;

    vector<float> boundary_value = {
         1000 - this->radius,
         1000 - this->radius,
         1000 - this->radius,
        -1000 + this->radius,
        -1000 + this->radius,
        -1000 + this->radius,
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

            glm::vec3 ratio = ((m1 - m2) / (m1 + m2)) * this->speed;

            this->speed = glm::normalize(glm::reflect(this->speed, boundary_normal[i])) * glm::length(ratio) * 0.8f;

            // float m1 = this->radius * this->radius * this->radius;
            // float m2 = numeric_limits<float>::max();

            // glm::vec3 to_wall = -boundary_normal[i];

            // glm::vec3 v1_c = glm::normalize(to_wall) * glm::dot(this->speed, glm::normalize(to_wall));
            // glm::vec3 v2_c = glm::vec3(0.0f, 0.0f, 0.0f);

            // glm::vec3 v1_u = this->speed - v1_c;

            // glm::vec3 calli_speed = ((m1 - m2) / (m1 + m2)) * v1_c + ((2 * m1) / (m1 + m2)) * v2_c;

            // this->speed = v1_u + calli_speed;

            // float m1 = this->radius * this->radius * this->radius;
            // float m2 = numeric_limits<float>::max();

            // glm::vec3 to_wall = -boundary_normal[i];

            // glm::vec3 v1 = glm::normalize(to_wall) * glm::dot(this->speed, glm::normalize(to_wall));
            // glm::vec3 v2 = glm::vec3(0.0f, 0.0f, 0.0f);

            // this->speed = this->speed - v1;

            // // callis.push_back((v1 * (m1 - m2) + 2 * m2 * v2) / (m1 + m2));
            // callis.push_back(-v1);

            ret = true;
        }
    }

    return ret;
}

bool Ball::check_ball_collision(Ball &ball)
{
    bool ret = false;

    if(glm::distance(this->position, ball.position) <= ball.radius + this->radius)
    {
        float m1 = this->radius * this->radius * this->radius;
        float m2 = ball.radius * ball.radius * ball.radius;

        glm::vec3 to_ball = ball.position - this->position;

        glm::vec3 v1 = glm::normalize(to_ball) * glm::dot(this->speed, glm::normalize(to_ball));
        glm::vec3 v2 = glm::normalize(-to_ball) * glm::dot(ball.speed, glm::normalize(-to_ball));

        this->position = ball.position + glm::normalize(-to_ball) * (ball.radius + this->radius);
        ball.position  = this->position + glm::normalize(to_ball) * (ball.radius + this->radius);

        // this->callis.push_back();
        // ball.callis.push_back();

        this->speed = this->speed - v1 + (v1 * (m1 - m2) + 2 * m2 * v2) / (m1 + m2) * 0.8f;
        ball.speed  = ball.speed  - v2 + (v2 * (m2 - m1) + 2 * m1 * v1) / (m1 + m2) * 0.8f;

        ret = true;
    }

    return ret;
}

void Ball::move(vector<Ball> &balls, bool cool)
{
    bool boundary_collide = check_boundary_collision(callis);
    // bool ball_collide = check_ball_collision(balls, callis);

    // if(boundary_collide || ball_collide)
    //     this->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    // else
    //     this->acceleration = glm::vec3(0.0f, -3.0f, 0.0f);

    // if(this->speed.x >= 0 && this->speed.x < 10)
    //     this->speed.x = 0;
    // if(this->speed.y >= 0 && this->speed.y < 10)
    //     this->speed.y = 0;
    // if(this->speed.z >= 0 && this->speed.z < 10)
    //     this->speed.z = 0;

    // if(abs(this->speed.x) < 2)
    //     this->speed.x = 0;
    // if(abs(this->speed.y) < 2)
    //     this->speed.y = 0;
    // if(abs(this->speed.z) < 2)
    //     this->speed.z = 0;

    if(cool)
        this->speed += this->acceleration;

    // this->speed *= 0.99;

    for(auto calli_speed : callis)
    {
        this->speed += calli_speed;
    }

    callis.clear();

    // cout << this->number << ": \t"  << (int)this->speed.x << " \t" << (int)this->speed.y << " \t" << (int)this->speed.z << endl;

    this->position += this->speed;
}

void Ball::draw(Shader shader, glm::vec3 player_position, float yaw, float pitch, float near, float far)
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
    static CULLING culling;

    culling = CULLING::INSIDE;

    for(auto plane : planes)
    {
        distance = glm::dot(position - player_position, plane);

        if(distance > radius)
        {
            culling = CULLING::OUTSIDE;

            break;
        }
        else if(abs(distance) < radius)
        {
            culling = CULLING::INTERSECT;
        }
    }

    if(culling != CULLING::OUTSIDE)
    {
        distance = glm::dot(position - player_position, glm::normalize(glm::cross(d - a, b - a))) - far;

        if(distance > radius)
        {
            culling = CULLING::OUTSIDE;
        }
        else if(abs(distance) < radius)
        {
            culling = CULLING::INTERSECT;
        }

        distance = glm::dot(position - player_position, glm::normalize(glm::cross(g - e, h - e))) - near;

        if(distance > radius)
        {
            culling = CULLING::OUTSIDE;
        }
        else if(abs(distance) < radius)
        {
            culling = CULLING::INTERSECT;
        }
    }

    BuildScene::render_ball(shader, vao, position, radius, size, culling);
}