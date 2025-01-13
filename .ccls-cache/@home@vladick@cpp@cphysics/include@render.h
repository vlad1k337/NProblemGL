#ifndef RENDER_H
#define RENDER_H
#include "glad/gl.h"
#include "spheres.h"

typedef struct SphereRenderer SphereRenderer;

SphereRenderer* createSpheresRenderer(GLuint shader, Sphere* sphere, float* offsets, const unsigned int offsets_count);
void destroySpheresRenderer(SphereRenderer* renderer);

void prepareSpheres(SphereRenderer* renderer);
void renderSpheres(SphereRenderer* renderer);

#endif
