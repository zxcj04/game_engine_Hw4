#include <BuildScene.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void BuildScene::set_viewport(SCENE scene, int width, int height)
{
    switch(scene)
    {
        case SCENE::FIRST:

            glViewport(0, 0, width * 3 / 4, height);

            break;

        case SCENE::ORTHO_X:

            glViewport(width * 3 / 4, height * 2 / 3, width * 1 / 4, height * 1 / 3);

            break;

        case SCENE::ORTHO_Y:

            glViewport(width * 3 / 4, height * 1 / 3, width * 1 / 4, height * 1 / 3);

            break;

        case SCENE::ORTHO_Z:

            glViewport(width * 3 / 4,              0, width * 1 / 4, height * 1 / 3);

            break;
    }
}

void BuildScene::set_projection(SCENE scene, glm::mat4 &projection, Camera camera, int width, int height, float near, float far)
{
    switch(scene)
    {
        case SCENE::FIRST:

            projection = glm::perspective(glm::radians(90.0f), (float)width / height * 3 / 4, near, far);

            break;

        case SCENE::ORTHO_X:
        case SCENE::ORTHO_Y:
        case SCENE::ORTHO_Z:

            if(width > height)
            {
                projection = glm::ortho(
                    camera.left                              , camera.right,
                    camera.bottom * ((float) height / width * 4 / 3) , camera.top * ((float) height / width * 4 / 3),
                    camera.near                              , camera.far
                );
            }
            else
            {
                projection = glm::ortho(
                    camera.left * ((float) width / height * 3 / 4)  , camera.right * ((float) width / height * 3 / 4),
                    camera.bottom                           , camera.top,
                    camera.near                             , camera.far
                );
            }

            // projection = glm::ortho(
            //     camera.left   , camera.right,
            //     camera.bottom , camera.top,
            //     camera.near   , camera.far
            // );

            break;
    }
}

void BuildScene::setup_boundary(unsigned int &vao_boundary)
{
    float vertices[] = {
        // positions          // normals          // color           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.5f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.5f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.5f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.5f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.5f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f, 0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f, 0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f, 0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f, 0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f
    };

    unsigned int VBO;

    glGenVertexArrays(1, &vao_boundary);
    glGenBuffers(1, &VBO);

    glBindVertexArray(vao_boundary);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}


void BuildScene::render_boundary(SCENE scene, unsigned int vao_boundary, Shader shader, unsigned int texture_wood, float size)
{
    glm::mat4 model = glm::mat4(1.0f);

    if(scene == SCENE::FIRST)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
    model = glm::scale(model, glm::vec3(size, size, size));

    shader.set_uniform("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_wood);

    shader.set_uniform("using_texture", 0);

    glBindVertexArray(vao_boundary);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
}

void BuildScene::setup_player(unsigned int &vao_player)
{
    float vertices[] = {
        // positions          // normals          // color           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f
    };

    unsigned int VBO;

    glGenVertexArrays(1, &vao_player);
    glGenBuffers(1, &VBO);

    glBindVertexArray(vao_player);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void BuildScene::render_player(SCENE scene, unsigned int vao_player, Shader shader, glm::vec3 position)
{
    if(scene == SCENE::FIRST)
        return;

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));

    shader.set_uniform("model", model);

    glBindVertexArray(vao_player);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void BuildScene::setup_ball(unsigned int &vao_player, int &size)
{
    vector<float> vertices;
    vector<float> tmp_vertices;

    int radius = 1;
    int sector_count = 24;
    int stack_count = 24;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, length_inv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sector_step = 2 * M_PI / sector_count;
    float stack_step = M_PI / stack_count;
    float sector_angle, stack_angle;

    for(int i = 0; i <= stack_count; ++i)
    {
        stack_angle = M_PI / 2 - i * stack_step;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stack_angle);             // r * cos(u)
        z = radius * sinf(stack_angle);              // r * sin(u)

        // add (sector_count+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sector_count; ++j)
        {
            sector_angle = j * sector_step;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sector_angle);             // r * cos(u) * cos(v)
            y = xy * sinf(sector_angle);             // r * cos(u) * sin(v)
            tmp_vertices.push_back(x);
            tmp_vertices.push_back(y);
            tmp_vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * length_inv;
            ny = y * length_inv;
            nz = z * length_inv;
            tmp_vertices.push_back(nx);
            tmp_vertices.push_back(ny);
            tmp_vertices.push_back(nz);

            // color
            tmp_vertices.push_back(1.0f);
            tmp_vertices.push_back(1.0f);
            tmp_vertices.push_back(1.0f);
            tmp_vertices.push_back(1.0f);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sector_count;
            t = (float)i / stack_count;
            tmp_vertices.push_back(s);
            tmp_vertices.push_back(t);
        }
    }

    vector<int> indices;

    int k1, k2;
    for(int i = 0; i < stack_count; ++i)
    {
        k1 = i * (sector_count + 1);     // beginning of current stack
        k2 = k1 + sector_count + 1;      // beginning of next stack

        for(int j = 0; j < sector_count; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stack_count-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    for(auto i: indices)
        for(int j = 0; j < 12; ++j)
            vertices.push_back(tmp_vertices[i*12 + j]);

    unsigned int VBO;

    glGenVertexArrays(1, &vao_player);
    glGenBuffers(1, &VBO);

    glBindVertexArray(vao_player);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(10 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    size = vertices.size() / 3;
}

void BuildScene::render_ball(Shader shader, unsigned int vao_ball, glm::vec3 position, float ball_radius, int size, CULLING culling)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(ball_radius, ball_radius, ball_radius));

    shader.set_uniform("model", model);

    switch(culling)
    {
        case CULLING::INSIDE:
            shader.set_uniform("culling", 0);

            break;
        case CULLING::INTERSECT:
            shader.set_uniform("culling", 1);

            break;
        case CULLING::OUTSIDE:
            shader.set_uniform("culling", 2);

            break;
    }

    glBindVertexArray(vao_ball);

    glDrawArrays(GL_TRIANGLES, 0, size);

    shader.set_uniform("culling", -1);
    glBindVertexArray(0);
}

void BuildScene::setup_view_volume(unsigned int &vao_view_volume)
{
    float vertices[] = {
        // positions    // color
         0,  0,  0,     1.0, 0.0, 0.0, 1.0,
        -1,  1,  1,     1.0, 0.0, 0.0, 1.0,
         1,  1,  1,     1.0, 0.0, 0.0, 1.0,

         0,  0,  0,     1.0, 0.0, 0.0, 1.0,
         1,  1,  1,     1.0, 0.0, 0.0, 1.0,
         1, -1,  1,     1.0, 0.0, 0.0, 1.0,

         0,  0,  0,     1.0, 0.0, 0.0, 1.0,
         1, -1,  1,     1.0, 0.0, 0.0, 1.0,
        -1, -1,  1,     1.0, 0.0, 0.0, 1.0,

         0,  0,  0,     1.0, 0.0, 0.0, 1.0,
        -1, -1,  1,     1.0, 0.0, 0.0, 1.0,
        -1,  1,  1,     1.0, 0.0, 0.0, 1.0,

        -1,  1,  1,     1.0, 0.0, 0.0, 1.0,
         1, -1,  1,     1.0, 0.0, 0.0, 1.0,
         1,  1,  1,     1.0, 0.0, 0.0, 1.0,

        -1,  1,  1,     1.0, 0.0, 0.0, 1.0,
        -1, -1,  1,     1.0, 0.0, 0.0, 1.0,
         1, -1,  1,     1.0, 0.0, 0.0, 1.0,
    };

    unsigned int VBO;

    glGenVertexArrays(1, &vao_view_volume);
    glGenBuffers(1, &VBO);

    glBindVertexArray(vao_view_volume);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void BuildScene::render_view_volume(SCENE scene, unsigned int vao_view_volume, Shader shader, glm::vec3 position, float yaw, float pitch, float near, float far)
{
    if(scene == SCENE::FIRST)
        return;

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0, 1, 0));
    model = glm::rotate(model, -glm::radians(pitch), glm::vec3(1, 0, 0));
    model = glm::scale(model, glm::vec3(far, far, far));

    shader.set_uniform("model", model);

    glBindVertexArray(vao_view_volume);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawArrays(GL_TRIANGLES, 0, 18);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
}

void BuildScene::setup_texture(unsigned int &texture, string file_path)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
}