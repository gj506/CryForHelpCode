#pragma once
#ifdef _WINDOWS
#include "GL/glew.h"
#endif
#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "ShaderProgram.h"
#include "Matrix.h"

class ClassDemoApp{
public:
	ClassDemoApp();
	~ClassDemoApp();

	void Setup();
	void ProcessEvents();
	bool UpdateAndRender();
	
	void Render();
	void Update(float elapsed);
	
	bool isDone();

	GLuint LoadTexture(const char* image_path);
private:
	// this is the setup for the translation and the rotation event handlers later on in the code
	float positionX = 0.0f;
	float positionY = 0.0f;
	float angle = 0.0f;
	GLuint emojiTexture;

	//this is for joystick input
	float xDir = 0.0f;
	float yDir = 0.0f;
	SDL_Event event;
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	SDL_Joystick *joystick;
	
	GLuint textureID;

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	
	ShaderProgram *program;
	
};