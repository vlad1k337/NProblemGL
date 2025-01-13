#ifndef WINDOW_H
#define WINDOW_H

typedef struct Window Window;

Window* createWindow(const char* window_title, int window_width, int window_height);

void swapWindow(Window* window_handle);
void destroyWindow(Window* window_handle);

#endif
