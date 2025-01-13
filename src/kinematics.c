#include "kinematics.h"
#include "cglm/vec3.h"
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define M_PI 3.14156
#define BIG_G 1.0f

KinematicBodies* createBodies(const unsigned int bodies_count)
{
    KinematicBodies* bodies = malloc(sizeof *bodies);
    bodies->count = bodies_count;
    bodies->positions  = malloc(3 * sizeof(float) * bodies_count);
    bodies->velocities = malloc(3 * sizeof(float) * bodies_count);
    bodies->masses     = malloc(sizeof(float) * bodies_count);

    return bodies;
}

void destroyBodies(KinematicBodies* bodies)
{
    free(bodies->positions);
    free(bodies->velocities);
    free(bodies->masses);
    free(bodies);

    bodies = NULL;
}

void loadInitialConditions(KinematicBodies* bodies, float* positions, float* velocities, float* masses)
{
    memcpy(bodies->positions, positions, sizeof(float) * 3 * bodies->count);
	memcpy(bodies->velocities, velocities, sizeof(float) * 3 * bodies->count);
	memcpy(bodies->masses, masses, sizeof(float) * bodies->count);
}

void initialConditionsSquare(KinematicBodies* bodies, const float min_boundary, const float max_boundary)
{
    srand(time(NULL));

    for(unsigned int i = 0, pos = 0; i < bodies->count; ++i, pos += 3)
    {

        bodies->positions[i][0] = (float)rand()/(float)RAND_MAX * (max_boundary - min_boundary) + min_boundary;
        bodies->positions[i][1] = (float)rand()/(float)RAND_MAX * (max_boundary - min_boundary) + min_boundary;
        bodies->positions[i][2] = (float)rand()/(float)RAND_MAX * (max_boundary - min_boundary) + min_boundary;

		glm_vec3_zero(bodies->velocities[i]);

        bodies->masses[i] = (float)rand()/(float)RAND_MAX;
	}
}

void initialConditionsBigBang(KinematicBodies* bodies, vec3 center, float radius)
{
	srand(time(NULL));

	glm_vec3_zero(bodies->positions[0]);
	glm_vec3_zero(bodies->velocities[0]);
	bodies->masses[0] = 1000.0f;

    for(unsigned int i = 1, pos = 0; i < bodies->count; ++i, pos += 3)
    {
        float phi   = rand() * M_PI;
        float theta = rand() * M_PI * 2.0;

        bodies->positions[i][0] = center[0] + radius * sin(phi) * cos(theta);
        bodies->positions[i][1] = center[1] + radius * sin(phi) * sin(theta);
        bodies->positions[i][2] = center[2] + radius * cos(phi);

        glm_vec3_zero(bodies->velocities[i]);

        bodies->masses[i] = 0.0;
	}
}

void initialConditionsDisk(KinematicBodies* bodies, vec3 center, float radius, float scale)
{
    glm_vec3_copy(center, bodies->positions[0]);
    glm_vec3_zero(bodies->velocities[0]);
    bodies->masses[0] = 1000.0;

    for(unsigned int i = 1; i < bodies->count; ++i)
    {
        float phi = rand() * 2 * M_PI;
        float r   = (float)rand()/(float)RAND_MAX;

        bodies->positions[i][0] = center[1] + r * radius * sin(phi) * scale;
        bodies->positions[i][1] = center[2] * scale;
        bodies->positions[i][2] = center[0] + r * radius * cos(phi) * scale;

		glm_vec3_zero(bodies->velocities[i]);

        bodies->masses[i] = 0.0;
    }
}

void simulateGravitational(KinematicBodies* bodies, float dt)
{
    float epsilon = 0.01;
	for(unsigned int i = 0; i < bodies->count; ++i)
    {
        vec3 acceleration;
        glm_vec3_zero(acceleration);
        for(unsigned int j = 0; j < bodies->count; ++j)
        {
            if(i != j)
            {
                float dist = glm_vec3_distance(bodies->positions[j], bodies->positions[i]);
                float field = BIG_G * (bodies->masses[j]) / pow(dist * dist + epsilon * epsilon, 1.5);

                vec3 displacement;
                glm_vec3_sub(bodies->positions[j], bodies->positions[i], displacement);
                glm_vec3_muladds(displacement, field, acceleration);
            }
        }
        glm_vec3_muladds(acceleration, dt * 0.5, bodies->velocities[i]);
        glm_vec3_muladds(bodies->velocities[i], dt, bodies->positions[i]);
		glm_vec3_muladds(acceleration, dt * 0.5, bodies->velocities[i]);
    }
}

void simulateFreeFall(KinematicBodies* bodies, float dt)
{
    for(unsigned int i = 0; i < bodies->count; ++i)
    {
        vec3 acceleration = {0.0, -9.8, 0.0};

        glm_vec3_muladds(acceleration, dt, bodies->velocities[i]);
        glm_vec3_muladds(bodies->velocities[i], dt, bodies->positions[i]);
    }
}

void calculateSphereSphereCollision(KinematicBodies* bodies, float radius)
{
    for(unsigned int i = 0; i < bodies->count; ++i)
    {
        for(unsigned int j = 0; j < bodies->count; ++j)
        {
            if(glm_vec3_distance(bodies->positions[i], bodies->positions[j]) < radius + radius)
            {
                vec3 normal;
                glm_vec3_sub(bodies->positions[j], bodies->positions[i], normal);
                glm_vec3_normalize(normal);

                vec3 rel;
                glm_vec3_sub(bodies->velocities[j], bodies->velocities[i], rel);

                float dot = glm_vec3_dot(rel, normal);

                vec3 impulse;
                glm_vec3_scale(normal, dot, impulse);

                glm_vec3_add(bodies->velocities[i], impulse, bodies->velocities[i]);
                glm_vec3_sub(bodies->velocities[j], impulse, bodies->velocities[j]);
            }
        }
    }
}

void calculateSphereBoxCollision(KinematicBodies* bodies, float min_boundary, float max_boundary)
{
    for(unsigned int i = 0; i < bodies->count; ++i)
    {
        if(bodies->positions[i][0] < min_boundary || bodies->positions[i][1] < min_boundary || bodies->positions[i][2] < min_boundary)
        {
            glm_vec3_negate(bodies->velocities[i]);
        }

        if(bodies->positions[i][0] > max_boundary || bodies->positions[i][1] > max_boundary || bodies->positions[i][2] > max_boundary)
        {
            glm_vec3_negate(bodies->velocities[i]);
        }
    }
}

void calculateSpherePlaneCollision(KinematicBodies* bodies, float plane_y, float radius, float elasticity)
{
    for(unsigned int i = 0; i < bodies->count; ++i)
    {
        if(fabsf(bodies->positions[i][1] - plane_y) < radius)
        {
            glm_vec3_negate(bodies->velocities[i]);
            glm_vec3_scale(bodies->velocities[i], elasticity, bodies->velocities[i]);
        }
    }
}
