#include "ClassDemoApp.h"

ClassDemoApp::ClassDemoApp(){ Setup(); }

void ClassDemoApp::Setup(){
	//SDL and OpenGl initialization
	//SDL_Init(SDL_INIT_VIDEO);

	// if we wanna handle controller input we gotta use SDL_INIT_JOYSTICK
	// SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	//then we use SDL_Joystick * playerOneCOntroller = SDL_JoystickOpen(0);
	//then the game loop
	//then we gotta clean up
	// SDL_JoystickClose(playerOneController);

	emojiTexture = LoadTexture("Smiley_Face");
	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0, 2.0, -1.0, 1.0);
	ShaderProgram myProgram = ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	//program  = ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	SDL_Joystick *joystick = SDL_JoystickOpen(0);


	glViewport(0, 0, 640, 360);

	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

}

bool ClassDemoApp::isDone(){ done = true; return done; }

ClassDemoApp::~ClassDemoApp(){
	//SDL and OpenGL cleanup (joysticks, textures, etc.
}


GLuint ClassDemoApp::LoadTexture(const char *image_path){
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	
	return textureID;
}

void ClassDemoApp::Render(){
	//clear, render, and swap the window

}

void ClassDemoApp::ProcessEvents(){
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = isDone();
		}
		//THIS IS FOR EVENT STUFF
		else if (event.type == SDL_MOUSEMOTION){
		// checks for mouse input and then we can translate the image based on these mouse inputs
		float unitX = (((float)event.motion.x / 640.0f) * 3.554) - 1.777f;
		float unitY = (((float)(360 - event.motion.y) / 360.0f) * 2.0) - 1.0f;

		positionX = unitX;
		positionY = unitY;

		}

		else if (event.type == SDL_MOUSEBUTTONDOWN){
		// this checks for mouse click and can be used to handle that event (we used it to rotate the image by 45 degrees)
		angle += 3.1415926536 * 0.25;
		}

		// we can now handle joystick stuff if we set up the joystick motion
		/*else if (event.type == SDL_JOYAXISMOTION){
			if (event.jaxis.which == 0){

				//printf("VAL: %d\n", event.jaxis.axis); quick debug tool to check values of joystick input

				if (event.jaxis.axis == 0){
					float floatVal = ((float)event.jaxis.value) / 32767.0f;

					if (fabs(floatVal) > 0.2f){
						xDir = floatVal;
					}
					else {
						xDir = 0.0f;
					}
					// this is to make the image ignore samll inputs
				}

				else if (event.jaxis.axis == 1){
					float floatVal = ((float)event.jaxis.value) / 32767.0f;
					if (fabs(floatVal) > 0.2f){
						yDir = floatVal;
					}
					else {
						yDir = 0.0f;
					}
				}
			}
			// same thing here
			else if (event.type == SDL_JOYBUTTONDOWN){
				if (event.jbutton.button == 0){
					angle += 3.1415926536 * 0.25f;
				}
				//this will handle joystick button pressing
			}
		}*/
	}
}

bool ClassDemoApp::UpdateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	Update(elapsed);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT);

	positionX += elapsed * 2.0f *xDir;
	positionY += elapsed * 2.0f *-yDir;

	modelMatrix.identity();
	modelMatrix.Translate(positionX, positionY, 0.0);
	modelMatrix.Rotate(angle);

	program->setModelMatrix(modelMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);

	glUseProgram(program->programID);

	//set it to the center
	float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	//THEY SHOULD BE DRAWN COUNTERCLOCKWISE

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	float texCoords[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	SDL_GL_SwapWindow(displayWindow);

	ProcessEvents();
	
	Render();

	return done;
}