#include "camera.h"
#include <SDL_events.h>
#include <SDL_timer.h>
#include <cglm/cglm.h>
#include <math.h>

static float getAspectRatio();

typedef enum
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STOP
} MovementDirection;

struct Camera {
    vec3 position;
    vec3 up;
    vec3 front;

    float yaw;
    float pitch;

    float aspect_ratio;
    float field_of_view;

    float mouse_sensitivity;

    float movement_speed;
    MovementDirection direction;

    mat4 matrix_look_at;
    mat4 matrix_projection;
};


Camera* createCamera()
{
    Camera* camera = malloc(sizeof *camera);

    camera->position[0] = 0.0f;
    camera->position[1] = 0.0f;
    camera->position[2] = 1.0f;

    camera->up[0] = 0.0f;
    camera->up[1] = 1.0f;
    camera->up[2] = 0.0f;

    camera->front[0] =  0.0f;
    camera->front[1] =  0.0f;
    camera->front[2] = -1.0f;

    camera->yaw   = -90.0f;
    camera->pitch =  0.0f;

    camera->aspect_ratio  = getAspectRatio();
    camera->field_of_view = 75.0f;

    camera->mouse_sensitivity = 0.1f;

    camera->movement_speed = 2.0f;
    camera->direction = STOP;

    updateCameraMatrices(camera);
    return camera;
}

void setPosition(Camera* camera, vec3 pos)
{
    glm_vec3_copy(pos, camera->position);
}

void updateCameraInput(Camera* camera, SDL_Event* event)
{
    if(event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        camera->aspect_ratio = getAspectRatio();
        return;
    }

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    if(keyboardState[SDL_SCANCODE_W])
    {
        camera->direction = FORWARD;
    }
    else if(keyboardState[SDL_SCANCODE_S])
    {
        camera->direction = BACKWARD;
    }
    else if(keyboardState[SDL_SCANCODE_A])
    {
        camera->direction = LEFT;
    }
    else if(keyboardState[SDL_SCANCODE_D])
    {
        camera->direction = RIGHT;
    } else {
        camera->direction = STOP;
    }

    if(event->type == SDL_MOUSEMOTION)
    {
        camera->yaw   += event->motion.xrel * camera->mouse_sensitivity;
        camera->pitch -= event->motion.yrel * camera->mouse_sensitivity;

        return;
    }
}

void updateCameraMatrices(Camera* camera)
{
    static Uint64 time_last = 0;
    Uint64 time_current = SDL_GetPerformanceCounter();
    float time_delta = (float)((time_current - time_last)/(float)SDL_GetPerformanceFrequency());
    time_last = time_current;

    switch(camera->direction)
    {
        case FORWARD:
        {
            vec3 offset;
            glm_vec3_scale(camera->front, camera->movement_speed * time_delta, offset);
            glm_vec3_add(camera->position, offset, camera->position);

            break;
        }

        case BACKWARD:
        {
            vec3 offset;
            glm_vec3_scale(camera->front, camera->movement_speed * time_delta, offset);
            glm_vec3_sub(camera->position, offset, camera->position);

            break;
        }

        case LEFT:
        {
            vec3 offset;
            glm_vec3_cross(camera->front, camera->up, offset);
            glm_vec3_normalize(offset);
            glm_vec3_scale(offset, camera->movement_speed * time_delta, offset);
            glm_vec3_sub(camera->position, offset, camera->position);

            break;
        }

        case RIGHT:
        {
            vec3 offset;
            glm_vec3_cross(camera->front, camera->up, offset);
            glm_vec3_normalize(offset);
            glm_vec3_scale(offset, camera->movement_speed * time_delta, offset);
            glm_vec3_add(camera->position, offset, camera->position);

            break;
        }

        case STOP:
        {
            break;
        }
    }


    if(camera->pitch > 89.0f)
    {
        camera->pitch = 89.0f;
    }
    if(camera->pitch < -89.0f)
    {
        camera->pitch = -89.0f;
    }

    camera->front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    camera->front[1] = sinf(glm_rad(camera->pitch));
    camera->front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));

    glm_vec3_normalize(camera->front);

    glm_look(camera->position, camera->front, camera->up, camera->matrix_look_at);
    glm_perspective(glm_rad(camera->field_of_view), camera->aspect_ratio, 0.1f, 1000.0f, camera->matrix_projection);

}

void destroyCamera(Camera *camera)
{
    free(camera);
}

float* getLookAt(Camera* camera)
{
    return camera->matrix_look_at[0];
}

float* getPerspective(Camera* camera)
{
    return camera->matrix_projection[0];
}

float* getPosition(Camera* camera)
{
    return camera->position;
}

static float getAspectRatio()
{
    SDL_Window* window = SDL_GL_GetCurrentWindow();

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    return (float)width / height;
}
