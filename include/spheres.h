#ifndef SPHERES_H
#define SPHERES_H
#include <cglm/cglm.h>

typedef struct SphereMesh
{
    unsigned int vertices_count;
    unsigned int indices_count;

    float* vertices;
    float* normals;
    unsigned int* indices;
} SphereMesh;

typedef struct Sphere
{
    float radius;
    SphereMesh mesh;
} Sphere;

Sphere* allocSphere(const float radius);
void    deallocSphere(Sphere* sphere);

void createSphereMesh(Sphere* sphere, const unsigned int sector_count, const unsigned int stack_count);

#endif
