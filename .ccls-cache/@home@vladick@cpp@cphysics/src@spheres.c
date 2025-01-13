#include "spheres.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323f
#endif

Sphere* allocSphere(const float radius)
{
    Sphere* sphere = (Sphere*)malloc(sizeof *sphere);
    sphere->radius = radius;
    sphere->mesh.vertices = NULL;
    sphere->mesh.normals  = NULL;
    sphere->mesh.indices  = NULL;
    return sphere;
}

void deallocSphere(Sphere* sphere)
{
    free(sphere->mesh.vertices);
    free(sphere->mesh.normals);
    free(sphere->mesh.indices);

    free(sphere);
}

/* UV-sphere */
void createSphereMesh(Sphere* sphere, const unsigned int sector_count, const unsigned int stack_count)
{
    /* Top and bottom stacks, together require sector_count * 3 * 2 indices, taking 3 indices per sector, in 2 rows (top and bottom).
	 * Every other row, using the same principle, requires (stack_count - 2) * sector_count * 6. We multiply by 6 as we now have 6 indicies per sector.
	 * vertices_count is taken with reserve at this point */


	sphere->mesh.vertices_count = 3 * sizeof(float) * (sector_count * (stack_count + 2) + 50);
    sphere->mesh.indices_count  = sizeof(unsigned int) * (sector_count * 6 + (stack_count - 2) * sector_count * 6);

    sphere->mesh.vertices = (float*)malloc(sphere->mesh.vertices_count);
    sphere->mesh.normals  = (float*)malloc(sphere->mesh.vertices_count);
    sphere->mesh.indices  = (unsigned int*)malloc(sphere->mesh.indices_count);

    printf("Vertecies: %d\n", sphere->mesh.vertices_count);
	printf("Indices: %d\n", sphere->mesh.indices_count);

	float sector_step = 2.0f * M_PI / sector_count;
    float stack_step = M_PI / stack_count;
    const float inv_radius = 1.0f / sphere->radius;

    float vertex_x, vertex_y, vertex_z, vertex_xy;
    float normal_x, normal_y, normal_z;

    float stack_angle, sector_angle;

    unsigned int vertex_count = 0;
    for(unsigned int i = 0; i <= stack_count; ++i)
    {
        stack_angle = M_PI / 2.0f - i * stack_step;
        vertex_xy = sphere->radius * cosf(stack_angle);
        vertex_z = sphere->radius * sinf(stack_angle);

        for(unsigned int j = 0; j <= sector_count; ++j)
        {
            sector_angle = j * sector_step;

            vertex_x = vertex_xy * cosf(sector_angle);
            vertex_y = vertex_xy * sinf(sector_angle);

            sphere->mesh.vertices[vertex_count] = vertex_x;
            sphere->mesh.vertices[vertex_count + 1] = vertex_y;
            sphere->mesh.vertices[vertex_count + 2] = vertex_z;

            normal_x = vertex_x * inv_radius;
            normal_y = vertex_y * inv_radius;
            normal_z = vertex_z * inv_radius;

            sphere->mesh.normals[vertex_count] = normal_x;
            sphere->mesh.normals[vertex_count + 1] = normal_y;
            sphere->mesh.normals[vertex_count + 2] = normal_z;
            vertex_count += 3;
        }
    }

	printf("%d\n", vertex_count);

    unsigned int indices_count = 0;
    for(unsigned int i = 0; i < stack_count; ++i)
    {
        unsigned int current_stack = i * (sector_count + 1);
        unsigned int next_stack = current_stack + sector_count + 1;

        for(unsigned int j = 0; j < sector_count; ++j, ++current_stack, ++next_stack)
        {
            if(i != 0)
            {
                sphere->mesh.indices[indices_count + 0] = current_stack;
                sphere->mesh.indices[indices_count + 1] = next_stack;
                sphere->mesh.indices[indices_count + 2] = current_stack + 1;
                indices_count += 3;
            }

            if(i != (stack_count - 1))
            {
                sphere->mesh.indices[indices_count + 0] = current_stack + 1;
                sphere->mesh.indices[indices_count + 1] = next_stack;
                sphere->mesh.indices[indices_count + 2] = next_stack + 1;
                indices_count += 3;
            }
        }
    }
    printf("Indices count: %d\n", indices_count);

}
