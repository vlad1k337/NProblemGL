#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

GLuint compileShader(const char* path, GLenum shader_type)
{
    FILE* shader_file = fopen(path, "rb");
    if(shader_file == NULL)
    {
        fprintf(stderr, "Shader file at %s not found\n", path);
        return 0;
    }
    fseek(shader_file, 0, SEEK_END);
    const long file_size = ftell(shader_file);
	rewind(shader_file);

    char* shader_source = malloc(file_size + 1);
	fread(shader_source, file_size, 1, shader_file);
	fclose(shader_file);
	shader_source[file_size] = '\0';

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char *const*)&shader_source, NULL);
    glCompileShader(shader);
	free(shader_source);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "Could not compile shader at %s:\n%s\n", path, info_log);
        return 0;
    }

    return shader;
}

GLuint linkShaderProgram(const unsigned int shader_count, ...)
{
    va_list shader_list;
    va_start(shader_list, shader_count);
    va_list shader_cleanup_list;
    va_copy(shader_cleanup_list, shader_list);

    GLuint shader_program = glCreateProgram();
    for(unsigned int i = 0; i < shader_count; ++i)
    {
        GLuint shader = va_arg(shader_list, GLuint);
        glAttachShader(shader_program, shader);
    }
    va_end(shader_list);
    glLinkProgram(shader_program);

    return shader_program;
}

void cleanShaderProgram(const unsigned int shader_count, ...)
{
    va_list shader_list;
    va_start(shader_list, shader_count);

    for(unsigned int i = 0; i < shader_count; ++i)
    {
        GLuint shader = va_arg(shader_list, GLuint);
        glDeleteShader(shader);
    }
}

extern void sendUniformMat4(GLuint shader_program, const GLchar *name, float *value);
