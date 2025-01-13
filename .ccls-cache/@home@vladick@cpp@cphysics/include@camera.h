#ifndef CAMERA_H
#define CAMERA_H

#include <SDL_events.h>
typedef struct Camera Camera;

Camera* createCamera();

void updateCameraInput(Camera* camera, SDL_Event* event);
void updateCameraMatrices(Camera* camera);
void destroyCamera(Camera* camera);

float* getLookAt(Camera* camera);
float* getPerspective(Camera* camera);

#endif
