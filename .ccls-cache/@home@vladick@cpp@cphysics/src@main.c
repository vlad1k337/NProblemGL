#include <stdbool.h>
#include <SDL_events.h>
#include <glad/gl.h>
#include <cglm/vec2.h>
#include "camera.h"
#include "spheres.h"
#include "window.h"
#include "shader.h"
#include "render.h"

int main()
{
	Window* window_handle = createWindow("SDL2 app", 800, 600);
	Camera* camera = createCamera();

	GLuint vertex   = compileShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragment = compileShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	GLuint shader   = linkShaderProgram(2, vertex, fragment);
	cleanShaderProgram(2, vertex, fragment);

	unsigned int offsets_count = 10;
	float* offsets = malloc(3 * sizeof(float) * offsets_count);

	Sphere* sphere = allocSphere(1.0);
	createSphereMesh(sphere, 36, 18);

	for(int i = -50, v = 0; i < 50; i += 10, v += 3)
	{
	    offsets[v + 0] = 0.0f;
		offsets[v + 1] = 0.0f;
		offsets[v + 2] = (float)i / 5.0f;
	}

	SphereRenderer* renderer = createSpheresRenderer(shader, sphere, offsets, offsets_count);
	free(offsets);

	bool quit = false;
	SDL_Event event;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while(!quit)
	{
		glClearColor(.1f, .1f, .1f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);

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

		prepareSpheres(renderer);
		sendUniformMat4(shader, "view", view);
		sendUniformMat4(shader, "proj", proj);

		renderSpheres(renderer);
		swapWindow(window_handle);
	}



	deallocSphere(sphere);
	destroySpheresRenderer(renderer);
	destroyCamera(camera);
	destroyWindow(window_handle);
}
