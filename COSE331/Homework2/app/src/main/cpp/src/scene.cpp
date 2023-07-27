#include "scene.h"
#include "binary/teapot.h"
#include "binary/rgb.h"
#include "binary/cloud.h"
#include "binary/tex_flower.h"
#include "checker.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::teapot = nullptr;
Texture* Scene::diffuse = nullptr;
Texture* Scene::dissolve = nullptr;
Material* Scene::material = nullptr;
Light* Scene::light = nullptr;

int Scene::width = 0;
int Scene::height = 0;

// Arcball variables
float lastMouseX = 0, lastMouseY = 0;
float currentMouseX = 0, currentMouseY = 0;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(20.0f, 30.0f, 20.0f);

    Scene::light = new Light(program);
    Scene::light->position = vec3(15.0f, 15.0f, 0.0f);

    //////////////////////////////
    /* TODO: Problem 2 : Change the texture of the teapot
     *  Modify and fill in the lines below.
     */
    for (auto& texel : rgbTexels) {
        if (texel.red == 0xFF){
            texel.red = 0x00;
            texel.blue = 0xFF;
        }
        else if (texel.blue == 0xFF){
            texel.blue = 0x00;
            texel.green = 0xFF;
        }
        else if (texel.green == 0xFF){
            texel.green = 0x00;
            texel.red = 0xFF;
        }
    }

    Scene::diffuse  = new Texture(Scene::program, 0, "textureDiff", rgbTexels, rgbSize);
//    Scene::diffuse  = new Texture(Scene::program, 0, "textureDiff", texFlowerData, texFlowerSize);
    //////////////////////////////

    Scene::material = new Material(Scene::program, diffuse, dissolve);
    Scene::teapot = new Object(program, material, teapotVertices, teapotIndices);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
    Scene::width = width;
    Scene::height = height;
}

void Scene::update(float deltaTime) {
    static float time = 0.0f;

    Scene::program->use();

    Scene::camera->update();
    Scene::light->update();

    Scene::teapot->draw();

    time += deltaTime;
}

void Scene::mouseDownEvents(float x, float y) {
    lastMouseX = currentMouseX = x;
    lastMouseY = currentMouseY = y;
}

void Scene::mouseMoveEvents(float x, float y) {
    //////////////////////////////
    /* TODO: Problem 3 : Implement Phong lighting
     *  Fill in the lines below.
     */
    currentMouseX = x;
    currentMouseY = y;
    int w = Scene::width;
    int h = Scene::height;

    vec3 eye = vec3(Scene::camera->eye);
    vec3 at = vec3(Scene::camera->at);
    vec3 up = vec3(Scene::camera->up);
    mat4 viewMat = lookAt(eye, at, up);

    //mapping [-1,1]
    float x_pre = (2 * lastMouseX) / w - 1;
    float y_pre = (2 * lastMouseY) / h - 1;
    float x_new = (2 * currentMouseX) / w - 1;
    float y_new = (2 * currentMouseY) / h - 1;

    vec2 q[2];
    q[0] = vec2(x_pre, y_pre);
    q[1] = vec2(x_new, y_new);
//    LOG_PRINT_DEBUG("last x %f, y %f", q[0].x, q[0].y);
//    LOG_PRINT_DEBUG("curr x %f, y %f", q[1].x, q[1].y);
    //making NDC v[0] = last, v[1] = curr
    vec3 v[2];
    for(int i = 0; i < 2; i++){
        if (dot(q[i], q[i]) > 1) v[i] = normalize(vec3(q[i].x, q[i].y, 0.0f));
        else v[i] = normalize(vec3(q[i].x, q[i].y, sqrt(1 - dot(q[i], q[i]))));
    }

    if (dot(v[0], v[1]) >= 1.0f) {
        lastMouseX = currentMouseX;
        lastMouseY = currentMouseY;
        return;
    }

    vec3 r_axis = mat3(inverse(viewMat)) * cross(v[0],v[1]);
    float r_angle = acos(dot(v[0], v[1]));
    mat4 rotateArc = glm::rotate(mat4(1.0f), r_angle, r_axis);

    vec4 lightPos = rotateArc * vec4(Scene::light->position, 1.0);

//    LOG_PRINT_DEBUG("light x %f, y %f, z %f", Scene::light->position.x, Scene::light->position.y, Scene::light->position.z);
    Scene::light->position = lightPos;

    lastMouseX = currentMouseX = x;
    lastMouseY = currentMouseY = y;
    //////////////////////////////
}