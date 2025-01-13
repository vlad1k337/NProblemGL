#include "render.h"
#include "spheres.h"

struct SphereRenderer
{
    GLuint shader;

    GLuint vertex_array;

    GLuint vertex_buffer;
    GLuint instance_buffer;
    GLuint index_buffer;

    GLsizei index_count;
    GLsizei instance_count;
};

SphereRenderer* createSpheresRenderer(GLuint shader, Sphere* sphere, float* offsets, unsigned int offsets_count)
{
    SphereRenderer* renderer = malloc(sizeof *renderer);
    renderer->shader = shader;
    renderer->index_count = sphere->mesh.indices_count / sizeof(unsigned int);
    renderer->instance_count = offsets_count;

    glGenVertexArrays(1, &renderer->vertex_array);
	glBindVertexArray(renderer->vertex_array);

	glGenBuffers(1, &renderer->index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere->mesh.indices_count, &sphere->mesh.indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &renderer->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sphere->mesh.vertices_count, &sphere->mesh.vertices[0], GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, sphere->mesh.vertices_count, &sphere->mesh.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &renderer->instance_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->instance_buffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * offsets_count, offsets, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribDivisor(1, 1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return renderer;
}

void destroySpheresRenderer(SphereRenderer* renderer)
{
    glUseProgram(0);
    glDeleteShader(renderer->shader);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &renderer->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &renderer->vertex_buffer);
    glDeleteBuffers(1, &renderer->instance_buffer);
    glDeleteBuffers(1, &renderer->index_buffer);

    free(renderer);
}

void prepareSpheres(SphereRenderer* renderer)
{
    glUseProgram(renderer->shader);
}

void renderSpheres(SphereRenderer* renderer)
{
    glBindVertexArray(renderer->vertex_array);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->index_buffer);
    glDrawElementsInstanced(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0, renderer->instance_count);
}
