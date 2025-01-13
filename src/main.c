#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <glad/gl.h>
#include <stdbool.h>
#include "camera.h"
#include "kinematics.h"
#include "spheres.h"
#include "window.h"
#include "shader.h"
#include "render.h"

int main()
{
	Window* window_handle = createWindow("SDL2 app", 800, 600);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Camera* camera = createCamera();
	float cam_pos[3] = {0.0, 0.0, 100.0};
	setPosition(camera, cam_pos);

	GLuint vertex   = compileShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragment = compileShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	GLuint shader   = linkShaderProgram(2, vertex, fragment);
	cleanShaderProgram(2, vertex, fragment);

	Sphere* sphere = allocSphere(1.0);
	createSphereMesh(sphere, 36, 18);

	KinematicBodies* bodies = createBodies(1500);
	float pos[9] = {0.0, 0.0, 0.0, 7.0, 0.0, 0.0, -7.0, 0.0, 0.0};
	float vel[9] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0};
	float masses[3] = {10.0, 0.0, 0.0};
	vec3 center;
	glm_vec3_zero(center);
	initialConditionsBigBang(bodies, center, 70.0);

	SphereRenderer* renderer = createSpheresRenderer(shader, &sphere->mesh, bodies->count);

	bool quit = false;
	SDL_Event event;

	uint64_t time_current = SDL_GetPerformanceCounter();
	uint64_t time_last = 0;
	double dt = 0.0f;

	while(!quit)
	{
    	time_last = time_current;
        time_current = SDL_GetPerformanceCounter();
        double dt = (double)((time_current - time_last)/(double)SDL_GetPerformanceFrequency());

		glClearColor(.1f, .1f, .1f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		while(SDL_PollEvent(&event))
		{
		    switch(event.type)
			{
			    case SDL_QUIT:
				{
				    quit = true;
					break;
				}

				case SDL_KEYDOWN:
				{
				    if(event.key.keysym.sym == SDLK_ESCAPE)
					{
					    quit = true;
					}
					break;
				}
			}
			updateCameraInput(camera, &event);
		}

		updateCameraMatrices(camera);
		float* view = getLookAt(camera);
		float* proj = getPerspective(camera);

		simulateGravitational(bodies, dt);

		prepareSpheres(renderer, bodies);

		sendUniformMat4(shader, "view", view);
		sendUniformMat4(shader, "proj", proj);

		renderSpheres(renderer);

		swapWindow(window_handle);
	}

	deallocSphere(sphere);
	destroyBodies(bodies);
	destroySpheresRenderer(renderer);
	destroyCamera(camera);
	destroyWindow(window_handle);
}
