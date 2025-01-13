#ifndef SHADER_H
#define SHADER_H

#include <cglm/mat4.h>
#include <glad/gl.h>

GLuint compileShader(const char* path, GLenum shader_type);
GLuint linkShaderProgram(const unsigned int shader_count, ...);

void cleanShaderProgram(const unsigned int shader_count, ...);

inline void sendUniformMat4(GLuint shader_program, const GLchar* name, float* value)
{
    GLint location = glGetUniformLocation(shader_program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

#endif
