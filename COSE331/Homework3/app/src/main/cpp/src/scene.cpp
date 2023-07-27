#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;

bool Scene::upperFlag = true;
bool Scene::lowerFlag = true;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));

    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
}

float elapsedUpperTime = 0.0f;
float elapsedLowerTime = 0.0f;
vector<Vertex> initPlayerVertices = playerVertices;

void Scene::update(float deltaTime) {
    Scene::program->use();
    Scene::camera->update();

    /*
     *
     * Write your code.
     *
     */

    vector<mat4> toBone, toParent;
    //lower -> 1:11, upper -> 12:27
    toParent.push_back(translate(jOffsets[0]));
    toBone.push_back(inverse(toParent[0]));
    for (int i = 1; i < 28; i++) {
        mat4 parentMat = toBone[jParents[i]];
        toParent.push_back(translate(jOffsets[i]));
        toBone.push_back(inverse(toParent[i]) * parentMat);
    }

    //upper 11-27
    if (Scene::upperFlag){
        elapsedUpperTime += deltaTime;
        if (elapsedUpperTime > 4) elapsedUpperTime -= 4;
    }
    //lower 0-10
    if(Scene::lowerFlag){
        elapsedLowerTime += deltaTime;
        if (elapsedLowerTime > 4) elapsedLowerTime -= 4;
    }
    vector<mat4> animationToRoot;
    int cUpperTime = (int)elapsedUpperTime, cLowerTime = (int)elapsedLowerTime,
        nUpperTime = (int)(elapsedUpperTime + 1)%4, nLowerTime = (int)(elapsedLowerTime+1)%4;
    //상하체 분리해서 움직임 넣기
    vector<float>   currUpperMot = motions[cUpperTime],
                    currLowerMot = motions[cLowerTime],
                    nextUpperMot = motions[nUpperTime],
                    nextLowerMot = motions[nLowerTime];

    mat4 currRotate = rotate(radians(currLowerMot[5]), vec3(0.0f, 0.0f, 1.0f))
                           * rotate(radians(currLowerMot[3]), vec3(1.0f, 0.0f, 0.0f))
                           * rotate(radians(currLowerMot[4]), vec3(0.0f, 1.0f, 0.0f));
    mat4 nextRotate = rotate(radians(nextLowerMot[5]), vec3(0.0f, 0.0f, 1.0f))
                           * rotate(radians(nextLowerMot[3]), vec3(1.0f, 0.0f, 0.0f))
                           * rotate(radians(nextLowerMot[4]), vec3(0.0f, 1.0f, 0.0f));
    quat currRotQuat = quat_cast(currRotate);
    quat nextRotQuat = quat_cast(nextRotate);
    quat intpolRotQuat = slerp(currRotQuat, nextRotQuat, elapsedLowerTime - (float)cLowerTime);
    mat4 intPolRot = mat4_cast(intpolRotQuat);

    mat4 interpolatedTranslate = translate(mix(vec3(currLowerMot[0], currLowerMot[1], currLowerMot[2]), vec3(nextLowerMot[0], nextLowerMot[1], nextLowerMot[2]), elapsedLowerTime - cLowerTime));
    animationToRoot.push_back(toParent[0] * interpolatedTranslate * intPolRot);
    //lower -> 0:5는 XYZ translation, XYZ rotation -> 1:11
    for(int i = 1; i < 12; i++){
        currRotate = rotate(radians(currLowerMot[i * 3 + 5]), vec3(0.0f, 0.0f, 1.0f))
                     * rotate(radians(currLowerMot[i * 3 + 3]), vec3(1.0f, 0.0f, 0.0f))
                     * rotate(radians(currLowerMot[i * 3 + 4]), vec3(0.0f, 1.0f, 0.0f));

        nextRotate = rotate(radians(nextLowerMot[i * 3 + 5]), vec3(0.0f, 0.0f, 1.0f))
                     * rotate(radians(nextLowerMot[i * 3 + 3]), vec3(1.0f, 0.0f, 0.0f))
                     * rotate(radians(nextLowerMot[i * 3 + 4]), vec3(0.0f, 1.0f, 0.0f));
    //quatarian
        currRotQuat = quat_cast(currRotate);
        nextRotQuat = quat_cast(nextRotate);
        intpolRotQuat = mix(currRotQuat, nextRotQuat, elapsedLowerTime - (float)cLowerTime);
        intPolRot = mat4_cast(intpolRotQuat);
        mat4 parentMat = animationToRoot[jParents[i]];
        animationToRoot.push_back(parentMat * toParent[i] * intPolRot);
    }
    //upper -> 12:27
    for(int i = 12; i < 28; i++){
        currRotate = rotate(radians(currUpperMot[i * 3 + 5]), vec3(0.0f, 0.0f, 1.0f))
                     * rotate(radians(currUpperMot[i * 3 + 3]), vec3(1.0f, 0.0f, 0.0f))
                     * rotate(radians(currUpperMot[i * 3 + 4]), vec3(0.0f, 1.0f, 0.0f));

        nextRotate = rotate(radians(nextUpperMot[i * 3 + 5]), vec3(0.0f, 0.0f, 1.0f))
                     * rotate(radians(nextUpperMot[i * 3 + 3]), vec3(1.0f, 0.0f, 0.0f))
                     * rotate(radians(nextUpperMot[i * 3 + 4]), vec3(0.0f, 1.0f, 0.0f));

        currRotQuat = quat_cast(currRotate);
        nextRotQuat = quat_cast(nextRotate);
        intpolRotQuat = mix(currRotQuat, nextRotQuat, elapsedUpperTime - (float)cUpperTime);
        intPolRot = mat4_cast(intpolRotQuat);

        mat4 parentMat = animationToRoot[jParents[i]];
        animationToRoot.push_back(parentMat * toParent[i] * intPolRot);
    }
    unsigned int pSize = playerVertices.size();
    for (int i = 0; i < pSize; i++) {
        vec3 pos = initPlayerVertices[i].pos, normal = initPlayerVertices[i].nor;
        vec4 weight = initPlayerVertices[i].weight;
        ivec4 bone = initPlayerVertices[i].bone;

        mat4 M = mat4(0.0f);

        for (int j = 0; j < 4; j++) {
            if (bone[j] == -1)
                continue;
            M += weight[j] * animationToRoot[bone[j]] * toBone[bone[j]];
        }

        playerVertices[i].pos = vec3(M * vec4(pos, 1.0f));
        playerVertices[i].nor = transpose(inverse(mat3(M))) * normal;
    }
    // Line Drawer
     glLineWidth(20);
     Scene::lineDraw->load({{vec3(-20.0f, 0.0f, 0.0f)}, {vec3(20.0f, 0.0f, 0.0f)}}, {0, 1});
     Scene::lineDraw->draw();

    Scene::player->load(playerVertices, playerIndices);
    Scene::player->draw();
}

void Scene::setUpperFlag(bool flag)
{
    Scene::upperFlag = flag;
}

void Scene::setLowerFlag(bool flag)
{
    Scene::lowerFlag = flag;
}