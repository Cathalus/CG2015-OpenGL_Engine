#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <SDL2\SDL.h>
#undef main
#include <iostream>
#include <string>

class Display
{
public:
	Display(std::string title, GLuint width, GLuint height, GLuint flags = 0);
	~Display();
	void update();
	void clear(float r, float g, float b, float a);
	void bindRenderTarget();
	inline bool isClosed() { return _isClosed; };
	inline SDL_Window* getWindow() { return _window; };
	inline int getWidth() { return *_width; };
	inline int getHeight() { return *_height; };
private:
	bool _isClosed = false;
	SDL_Window* _window;
	SDL_GLContext _context;
	GLint* _width = new GLint(0);
	GLint* _height = new GLint(0);
};

