#include "scene.h"

#include "obj_teapot.h"
#include "tex_flower.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Light* Scene::light = nullptr;
Object* Scene::teapot = nullptr;
Material* Scene::flower = nullptr;
float Scene::time = 0.0f;

void Scene::setup(AAssetManager* aAssetManager) {

    // set asset manager
    Asset::setManager(aAssetManager);

    // create shaders
    vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    // create program
    program = new Program(vertexShader, fragmentShader);

    // create camera
    camera = new Camera(program);
    camera->eye = vec3(60.0f, 60.0f, 60.0f);
    camera->cameraN = glm::normalize(camera->eye - camera->at);
    camera->cameraU = glm::normalize(glm::cross(camera->up, camera->cameraN));
    camera->cameraV = glm::normalize(glm::cross(camera->cameraN, camera->cameraU));

    // create light
    light = new Light(program);
    light->position = vec3(100.0f, 0.0f, 0.0f);

    // create floral texture
    flower = new Material(program, texFlowerData, texFlowerSize);

    // create teapot object
    teapot = new Object(program, flower, objTeapotVertices, objTeapotIndices,
                        objTeapotVerticesSize, objTeapotIndicesSize);
}

void Scene::screen(int width, int height) {

    // set camera aspect ratio
    camera->aspect = (float) width / height;
}

void Scene::update(float deltaTime) {
    static float radius = 15.0f;

    // use program
    program->use();

    // scale teapot
    float scale = 0.5;
    mat4 scaleM;
    scaleM = transpose(mat4(scale, 0.0f, 0.0f, 0.0f,
                            0.0f, scale, 0.0f, 0.0f,
                            0.0f, 0.0f, scale, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f));
    teapot->worldMatrix = scaleM;

    //////////////////////////////
    /* TODO: Problem 2.
    *  Fill in the lines below.
    */
    // TODO : Make the teapot orbit on YZ plane
    float orbitSpeed = 0.015f;  //공전 스피드
    float orbitAngle = time * orbitSpeed;
    vec3 axisXonYZ = vec3(0.0f, radius * cos(orbitAngle), radius * sin(orbitAngle));
    mat4 orbitM;
    orbitM = translate(mat4(1.0f), axisXonYZ);

    // TODO : Rotate the teapot around its own y-axis
    float spinSpeed = 0.03f;    //자전스피드 -> 공전스피드의 두배
    float spinAngle = time * spinSpeed;
    mat4 spinM;
    spinM = transpose(mat4(cos(spinAngle), 0.0f, sin(spinAngle), 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           -sin(spinAngle), 0.0f, cos(spinAngle), 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f));
    teapot->worldMatrix = orbitM * spinM * teapot->worldMatrix;

    //////////////////////////////

    camera->updateViewMatrix();
    camera->updateProjectionMatrix();

    teapot->viewMatrix = camera->viewMatrix;
    teapot->projMatrix = camera->projMatrix;

    light->setup();

    // increase time
    time = time + deltaTime * 100;

    // draw teapot
    teapot->draw();
}

void Scene::dragScreen(float dx,float dy)
{
    float Sensitivity = 0.1;

    float thetaYaw=glm::radians(Sensitivity*dx);

    moveCamera(thetaYaw);
}
void Scene::moveCamera(float theta)
{
    //////////////////////////////
    /* TODO: Problem 3.
     *  Note that u,v,n should always be orthonormal.
     *  The u vector can be accessed via camera->cameraU.
     *  The v vector can be accessed via camera->cameraV.
     *  The n vector can be accessed via camera->cameraN.
     *  === HINT ===
     *  Initial camera position is (60.0f, 60.0f, 60.0f)
     *  Argument theta is amount of camera rotation in radians
     */

    vec3 prevEye = camera->eye; //원래있던거 가져오기
    float cosT = cos(theta);
    float sinT = sin(theta);
    vec3 newEye = vec3(cosT * prevEye.x + sinT * prevEye.z, 60.0f, cosT * prevEye.z - sinT * prevEye.x);

    vec3 newAt = vec3(0.0f, 0.0f, 0.0f);    //카메라 보는 곳은 안바뀜
    vec3 newUp = vec3(0.0f, 1.0f, 0.0f);    //y축도 안바뀜
    vec3 newN = normalize(newEye - newAt);          //y좌표는 60으로 일정
    vec3 newU = normalize(cross(newUp, newN));      //cross product로 구해줌
    vec3 newV = normalize(cross(newN, newU));       //V = n X u

    camera->eye = newEye;
    camera->at = newAt;
    camera->up = newUp;
    camera->cameraU = newU;
    camera->cameraV = newV;
    camera->cameraN = newN;

//////////////////////////////

}






































