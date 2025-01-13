#ifndef RENDER_H
#define RENDER_H
#include "glad/gl.h"
#include "kinematics.h"
#include "spheres.h"

typedef struct SphereRenderer SphereRenderer;

SphereRenderer* createSpheresRenderer(GLuint shader, const SphereMesh* mesh, const unsigned int instance_count);
void destroySpheresRenderer(SphereRenderer* renderer);

void prepareSpheres(SphereRenderer* renderer, KinematicBodies* bodies);
void renderSpheres(SphereRenderer* renderer);

#endif
