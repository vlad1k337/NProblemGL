#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <cglm/vec3.h>

typedef struct
{
    unsigned int count;
    vec3* positions;
    vec3* velocities;
    float* masses;
} KinematicBodies;

KinematicBodies* createBodies(const unsigned int bodies_count);
void             destroyBodies(KinematicBodies* bodies);

void simulateGravitational(KinematicBodies* bodies, float dt);
void simulateFreeFall(KinematicBodies* bodies, float dt);

void initialConditionsSquare(KinematicBodies* bodies, const float min_boundary, const float max_boundary);
void initialConditionsBigBang(KinematicBodies* bodies, vec3 center, float radius);
void initialConditionsDisk(KinematicBodies* bodies, vec3 center, float radius, float scale);

void loadInitialConditions(KinematicBodies* bodies, float* positions, float* velocities, float* masses);

void calculateSphereSphereCollision(KinematicBodies* bodies, float radius);
void calculateSphereBoxCollision(KinematicBodies* bodies, float min_boundary, float max_boundary);
void calculateSpherePlaneCollision(KinematicBodies* bodies, float plane_y, float radius, float elasticity);

#endif
