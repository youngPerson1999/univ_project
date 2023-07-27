#ifndef HW2_CAMERA_H
#define HW2_CAMERA_H

#include "global.h"
#include "program.h"

class Camera {
public:
    vec3 eye, at, up;
    float fovy, aspect, zNear, zFar;

    Camera(Program* program);

    virtual void update() const;

private:
    Program* program;

    virtual void create(Program* program);
};

#endif //HW2_CAMERA_H
