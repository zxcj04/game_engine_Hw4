#include <BuildScene.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void BuildScene::set_viewport(SCENE scene, int width, int height)
{
    switch(scene)
    {
        case SCENE::FIRST:

            glViewport(0, height / 2, width / 2, height / 2);

            break;

        case SCENE::ORTHO_X:

            glViewport(0, 0, width / 2, height / 2);

            break;

        case SCENE::ORTHO_Y:

            glViewport(width / 2, height / 2, width / 2, height / 2);

            break;

        case SCENE::ORTHO_Z:

            glViewport(width / 2, 0, width / 2, height / 2);

            break;
    }
}

void BuildScene::set_projection(SCENE scene, glm::mat4 &projection, Camera camera, int width, int height)
{
    switch(scene)
    {
        case SCENE::FIRST:

            projection = glm::perspective(glm::radians(90.0f), (float)width / height, 105.0f, 100000.0f);

            break;

        case SCENE::ORTHO_X:
        case SCENE::ORTHO_Y:
        case SCENE::ORTHO_Z:

            if(width > height)
            {
                projection = glm::ortho(
                    camera.left                              , camera.right,
                    camera.bottom * ((float) height / width) , camera.top * ((float) height / width),
                    camera.near                              , camera.far
                );
            }
            else
            {
                projection = glm::ortho(
                    camera.left * ((float) width /height)  , camera.right * ((float) width /height),
                    camera.bottom                          , camera.top,
                    camera.near                            , camera.far
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


void BuildScene::render_boundary(SCENE scene, unsigned int vao_boundary, Shader shader, unsigned int texture_wood)
{
    glm::mat4 model = glm::mat4(1.0f);

    if(scene == SCENE::FIRST)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
    model = glm::scale(model, glm::vec3(2000.0f, 2000.0f, 2000.0f));

    shader.set_uniform("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_wood);

    shader.set_uniform("texture_wood", 0);

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

void BuildScene::render_player(unsigned int vao_player, Shader shader, glm::vec3 position)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));

    shader.set_uniform("model", model);

    glBindVertexArray(vao_player);

    glDrawArrays(GL_TRIANGLES, 0, 36);
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
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}