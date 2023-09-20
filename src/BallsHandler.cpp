#include <BallsHandler.hpp>

bool to_near_to_another_cube(vector<Cube> cubes, glm::vec3 position, float length)
{
    bool ret = false;

    for(auto cube: cubes)
    {
        if(cube.number == Cube::count)
            continue;

        if(glm::length(cube.position - position) < (cube.length / 2 + length / 2) * sqrt(2))
        {
            ret = true;
            break;
        }
    }

    return ret;
}

void create_maze(vector<vector<int>> &maze, int x, int y, int height, int width)
{
    static int offsetX[4] = {-2, 2, 0, 0};
    static int offsetY[4] = {0, 0, -2, 2};
    int count = 0;
    int i, random;

    while(1)
    {
        count = 0;
        for(i = 0 ; i < 4 ; i++)
            if( x + offsetX[i] >= 0 && x + offsetX[i] <= height &&
                y + offsetY[i] >= 0 && y + offsetY[i] <= width &&
                maze[x + offsetX[i]][y + offsetY[i]] == -1
              )
                count++;

        // cout << "( " << x << ", " << y << " ): " << count << endl;

        if(count == 0)
            return;

        do
        {
            random = rand() % 4;
        }
        while(x + offsetX[random] < 0 || x + offsetX[random] > height ||
              y + offsetY[random] < 0 || y + offsetY[random] > width  ||
              maze[x + offsetX[random]][y + offsetY[random]] != -1);

        maze[x + offsetX[random]][y + offsetY[random]] = 1;
        maze[x + (offsetX[random] / 2)][y + (offsetY[random] / 2)] = 1;

        create_maze(maze, x + offsetX[random], y + offsetY[random], height, width);
    }
}

vector<vector<int>> generate_maze(float width, float height)
{
    vector<vector<int>> maze;

    maze.assign(width, vector<int>(height, 0));

    for(int w = 0; w < width + 1; w++)
        for(int h = 0; h < height + 1; h++)
            if(h % 2 == 1 && w % 2 == 1)
                maze[h][w] = -1;  // avail

    create_maze(maze, 1, 1, height, width);

    return maze;

}

BallsHandler::BallsHandler(float boundary_size)
{
    random_device rd;

    rd_generator = mt19937(rd());

    gap = 500;

    regular_grid = vector<vector<vector<set<int>>>>(boundary_size/gap + 2, vector<vector<set<int>>>(boundary_size/gap + 2, vector<set<int>>(boundary_size/gap + 2, set<int>())));
    cube_regular_grid = vector<vector<vector<set<int>>>>(boundary_size/gap + 2, vector<vector<set<int>>>(boundary_size/gap + 2, vector<set<int>>(boundary_size/gap + 2, set<int>())));

    this->maze_width = boundary_size / 500.0f;
    this->maze_height = boundary_size / 500.0f;

    this->maze = generate_maze(maze_width, maze_height);
    calc_movement();

    float gap = 500.0f;

    for(size_t h = 0; h < this->maze.size(); h++)
        for(size_t w = 0; w < this->maze[h].size(); w++)
            if(this->maze[h][w] != 1)
                add_cube(boundary_size, h * gap, w * gap);
}

BallsHandler::~BallsHandler()
{

}

void BallsHandler::add_ball(glm::vec3 position, float radius, glm::vec3 speed, TYPE type)
{
    position.y = -1000 + radius;
    _balls.push_back(Ball(position, radius, speed, type));
}

void BallsHandler::add_ball(float boundary_size)
{
    uniform_real_distribution<float> unif_position(-boundary_size / 2, boundary_size / 2);
    uniform_real_distribution<float> unif_radius(50, 175);
    uniform_real_distribution<float> unif_speed(-50, 50);
    uniform_int_distribution<int> unif_type(0, 2);

    float radius = 75.0f;

    int type = unif_type(rd_generator);

    glm::vec3 position;
    glm::vec3 speed;

    do
    {
        position.x = unif_position(rd_generator);
        position.y = radius;
        position.z = unif_position(rd_generator);

    } while (abs(position.x) + radius >= boundary_size / 2 ||
             abs(position.y) + radius >= boundary_size / 2 ||
             abs(position.z) + radius >= boundary_size / 2 ||
             this->maze[(position.x + boundary_size / 2) / 500][(position.z + boundary_size / 2) / 500] != 1);

    speed = glm::vec3( 0, 0, 0 );

    add_ball(position, radius, speed, (TYPE) type);
}

void BallsHandler::add_cube(float boundary_size, float x, float y)
{
    float length = 500;
    float rotate = 0;
    glm::vec3 position;

    position = glm::vec3(x + length / 2 - boundary_size / 2, 0, y + length / 2 - boundary_size / 2);
    position.y = -boundary_size / 4 / 2 + length / 2;

    Cube cube = Cube(position, length, rotate);

    _cubes.push_back(cube);
}

void BallsHandler::draw_balls(Shader shader, unsigned int texture_ball, glm::vec3 player_position, float yaw, float pitch, float near, float far)
{
    for(auto & ball: _balls)
    {
        if(ball.deleted)
            continue;

        ball.draw(shader, texture_ball, player_position, yaw, pitch, near, far);
    }
}

int can_move(vector<vector<int>> &maze, int maze_width, int maze_height, int x, int y, int direction, int times)
{
    switch(direction)
    {
        case 0:
            y -= 1;
            break;
        case 1:
            y += 1;
            break;
        case 2:
            x += 1;
            break;
        case 3:
            x -= 1;
            break;
    }

    if(x < 0 || x > maze_width - 1 ||
       y < 0 || y > maze_height - 1)
        return 0;

    if(maze[x][y] != 1)
        return 0;

    if(times == 0)
        return 1;

    int ret = 0;

    if(direction != 0)
        ret = max(ret, can_move(maze, maze_width, maze_height, x, y, 0, times - 1));
    if(direction != 1)
        ret = max(ret, can_move(maze, maze_width, maze_height, x, y, 1, times - 1));
    if(direction != 2)
        ret = max(ret, can_move(maze, maze_width, maze_height, x, y, 2, times - 1));
    if(direction != 3)
        ret = max(ret, can_move(maze, maze_width, maze_height, x, y, 3, times - 1));

    return ret + times;
}

bool can_see(vector<vector<int>> &maze, glm::ivec2 grid_position, glm::ivec2 grid_target)
{
    if(grid_position.x != grid_target.x && grid_position.y != grid_target.y)
        return false;

    if(grid_position.x == grid_target.x)
    {
        int start = min(grid_position.y, grid_target.y);
        int end = max(grid_position.y, grid_target.y);

        int x = grid_position.x;

        for(int i = start ; i <= end ; i++)
            if(maze[x][i] < 1)
                return false;
    }
    else
    {
        int start = min(grid_position.x, grid_target.x);
        int end = max(grid_position.x, grid_target.x);

        int y = grid_position.y;

        for(int i = start ; i <= end ; i++)
            if(maze[i][y] < 1)
                return false;
    }

    return true;
}

void BallsHandler::calc_movement()
{
    maze_movement.clear();

    maze_movement.assign(maze_width, vector<vector<glm::vec2>>(maze_height, vector<glm::vec2>()));

    for(size_t w = 0 ; w < maze.size() ; w++)
    {
        for(size_t h = 0 ; h < maze[w].size() ; h++)
        {
            glm::vec2 now = glm::ivec2(w, h);

            for(size_t tw = 0 ; tw < maze.size() ; tw++)
            {
                for(size_t th = 0 ; th < maze[tw].size() ; th++)
                {
                    glm::vec2 target = glm::ivec2(tw, th);

                    bool see = can_see(maze, now, target);

                    if(see)
                    {
                        maze_movement[w][h].push_back(target);
                    }
                }
            }
        }
    }
}

void BallsHandler::move_balls(float decay, bool spatial_partition, bool shrink)
{
    vector<int> del;

    if(spatial_partition)
    {
        for(int i = 1; i <= 8000 / gap; i++)
        for(int j = 1; j <= 8000 / gap; j++)
        for(int k = 1; k <= 8000 / gap; k++)
        for(auto one: regular_grid[i][j][k])
        {
            for(int i2 = i - 1; i2 <= i + 1; i2++)
            for(int j2 = j - 1; j2 <= j + 1; j2++)
            for(int k2 = k - 1; k2 <= k + 1; k2++)
            {
                for(auto two: regular_grid[i2][j2][k2])
                {
                    if(one == two)
                        continue;

                    if(_balls[one].deleted || _balls[two].deleted)
                        continue;

                    bool colli = _balls[one].check_ball_collision(_balls[two], decay);

                    if(colli && _balls[one].type != _balls[two].type)
                    {
                        if(_balls[two].type == TYPE::FOOD)
                        {
                            if(_balls[one].radius < 225)
                                _balls[one].radius += 25;
                            else
                            {
                                _balls[one].radius -= 50;

                                add_ball(_balls[one].position, 75.0f, glm::vec3(0, 0, 0), _balls[one].type);
                            }

                            _balls[two].deleted = true;

                            del.push_back(two);
                        }
                        else
                        {
                            int bigger, smaller;

                            if(_balls[one].radius > _balls[two].radius)
                            {
                                bigger = one;
                                smaller = two;
                            }
                            else if(_balls[one].radius < _balls[two].radius)
                            {
                                bigger = two;
                                smaller = one;
                            }
                            else
                            {
                                if(rand() > 0.5)
                                {
                                    bigger = two;
                                    smaller = one;
                                }
                                else
                                {
                                    bigger = two;
                                    smaller = one;
                                }
                            }

                            _balls[bigger].radius += _balls[smaller].radius;

                            if(_balls[bigger].radius > 225)
                            {
                                add_ball(_balls[bigger].position, _balls[bigger].radius / 2, glm::vec3(0, 0, 0), _balls[bigger].type);

                                _balls[bigger].radius /= 2;
                            }

                            _balls[smaller].deleted = true;

                            del.push_back(smaller);
                        }
                    }
                }
            }

            for(int i2 = i - 1; i2 <= i + 1; i2++)
            for(int j2 = j - 1; j2 <= j + 1; j2++)
            for(int k2 = k - 1; k2 <= k + 1; k2++)
            {
                for(auto two: cube_regular_grid[i2][j2][k2])
                {
                    _balls[one].check_cube_collision(_cubes[two], decay);
                }
            }
        }

    }
    else
    {
        for(int i = 0; i < _balls.size(); i++)
            for(int j = i + 1; j < _balls.size() ; j++)
            {
                _balls[i].check_ball_collision(_balls[j], decay);
            }

        for(int i = 0; i < _balls.size(); i++)
            for(int j = 0; j < _cubes.size() ; j++)
            {
                _balls[i].check_cube_collision(_cubes[j], decay);
            }
    }

    for(auto to_delete: del)
    {
        Ball del_ball = _balls[to_delete];

        regular_grid[del_ball.grid_position.x][del_ball.grid_position.y][del_ball.grid_position.z].erase(del_ball.number);

        // _balls.erase(del_ball);

        _balls[to_delete].deleted = true;
    }

    red_ball_quantity = 0;
    green_ball_quantity = 0;
    blue_ball_quantity = 0;

    for(auto & ball: _balls)
    {
        if(ball.deleted)
            continue;

        if(spatial_partition)
            ball.update_regular_grid(regular_grid, gap);

        if(ball.type == TYPE::RANDOM)
            red_ball_quantity++;
        else if(ball.type == TYPE::AI)
            green_ball_quantity++;
        else if(ball.type == TYPE::FOOD)
            blue_ball_quantity++;

        ball.move(_balls, decay, maze_movement, regular_grid, shrink);
    }

    for(auto & cube: _cubes)
    {
        if(spatial_partition)
            cube.update_regular_grid(cube_regular_grid, gap);

        // cube.move(cube, decay);
    }
}

void BallsHandler::reset_balls()
{
    _balls.clear();

    for(int i = 0; i <= 8000 / gap + 1; i++)
    for(int j = 0; j <= 8000 / gap + 1; j++)
    for(int k = 0; k <= 8000 / gap + 1; k++)
    {
        regular_grid[i][j][k].clear();
    }

    Ball::count = 0;
}

void BallsHandler::reset_cubes(float boundary_size)
{
    _cubes.clear();

    for(int i = 0; i <= 8000 / gap + 1; i++)
    for(int j = 0; j <= 8000 / gap + 1; j++)
    for(int k = 0; k <= 8000 / gap + 1; k++)
    {
        cube_regular_grid[i][j][k].clear();
    }

    Cube::count = 0;

    // for(int i = 0 ; i < 15 ; i++)
    // {
    //     add_cube(boundary_size);
    // }

    this->maze = generate_maze(maze_width, maze_height);
    calc_movement();

    float gap = 500.0f;

    for(size_t h = 0; h < this->maze.size(); h++)
        for(size_t w = 0; w < this->maze[h].size(); w++)
            if(this->maze[h][w] != 1)
                add_cube(boundary_size, h * gap, w * gap);
}

void BallsHandler::draw_cubes(Shader shader, unsigned int texture_cube, bool show)
{
    if(show)
    for(auto & cube: _cubes)
        cube.draw(shader, texture_cube);
}

void BallsHandler::add_food()
{
    static int food_clock = 50;

    if(food_clock == 0)
    {
        for(int i = 0 ; i < 2 ; i++)
        {
                float boundary_size = 8000.0f;

                uniform_real_distribution<float> unif_position(-boundary_size / 2, boundary_size / 2);
                uniform_real_distribution<float> unif_radius(50, 175);
                uniform_real_distribution<float> unif_speed(-50, 50);

                float radius = 75.0f;

                TYPE type = TYPE::FOOD;

                glm::vec3 position;
                glm::vec3 speed;

                do
                {
                    position.x = unif_position(rd_generator);
                    position.y = radius;
                    position.z = unif_position(rd_generator);

                } while (abs(position.x) + radius >= boundary_size / 2 ||
                        abs(position.y) + radius >= boundary_size / 2 ||
                        abs(position.z) + radius >= boundary_size / 2 ||
                        this->maze[(position.x + boundary_size / 2) / 500][(position.z + boundary_size / 2) / 500] != 1);

                speed = glm::vec3( 0, 0, 0 );

                add_ball(position, radius, speed, type);
        }

        food_clock = 50;
    }
    else
        food_clock--;
}