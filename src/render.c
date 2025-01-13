#include "render.h"
#include "kinematics.h"
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

SphereRenderer* createSpheresRenderer(GLuint shader, const SphereMesh* mesh, const unsigned int instance_count)
{
    SphereRenderer* renderer = malloc(sizeof *renderer);
    renderer->shader = shader;
    renderer->index_count = mesh->indices_count / sizeof(unsigned int);
    renderer->instance_count = instance_count;

    glGenVertexArrays(1, &renderer->vertex_array);
	glBindVertexArray(renderer->vertex_array);

	glGenBuffers(1, &renderer->index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_count, mesh->indices, GL_STATIC_DRAW);

	glGenBuffers(1, &renderer->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices_count, mesh->vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &renderer->instance_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->instance_buffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * instance_count, NULL, GL_DYNAMIC_DRAW);
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDeleteVertexArrays(1, &renderer->vertex_array);
    glDeleteBuffers(1, &renderer->vertex_buffer);
    glDeleteBuffers(1, &renderer->instance_buffer);
    glDeleteBuffers(1, &renderer->index_buffer);

    free(renderer);
}

void prepareSpheres(SphereRenderer* renderer, KinematicBodies* bodies)
{
    glUseProgram(renderer->shader);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->instance_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * bodies->count, bodies->positions);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void renderSpheres(SphereRenderer* renderer)
{
    glBindVertexArray(renderer->vertex_array);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->index_buffer);
    glDrawElementsInstanced(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0, renderer->instance_count);
}
