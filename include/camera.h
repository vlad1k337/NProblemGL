#ifndef CAMERA_H
#define CAMERA_H

#include <SDL_events.h>
#include <cglm/cglm.h>

typedef struct Camera Camera;

Camera* createCamera();

void setPosition(Camera* camera, vec3 pos);

void updateCameraInput(Camera* camera, SDL_Event* event);
void updateCameraMatrices(Camera* camera);
void destroyCamera(Camera* camera);

float* getLookAt(Camera* camera);
float* getPerspective(Camera* camera);
float* getPosition(Camera* camera);

#endif
