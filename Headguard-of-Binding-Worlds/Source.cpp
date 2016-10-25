

// OpenGL headers
#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/GL.h>

#include <iostream>

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>

//Program headers
#include "Model.h"
#include "Shader.h"
#include "Entity.h"
#include "ms3dModelLoader.h"
#include "CharacterBuilder.h"
#include "EntityHandler.h"
#include "ModelHandler.h"
#include "ArmatureHandler.h"
#include "TextureHandler.h"

int WIDTH = 1024;
int HEIGHT = 768;

vector<Entity> _entities;
ModelHandler _modelHandler = ModelHandler();
TextureHandler _textureHandler = TextureHandler();
ArmatureHandler _armatureHandler = ArmatureHandler();

bool quit;

SDL_Window* window;
SDL_GLContext glContext;

Shader shader;
ms3dModelLoader modelLoader;
Entity entity1;
Entity entity2;
double deltaTime = 0;


using namespace std;

int main(int argc, char *argv[])
{
    quit = false;
	
    //Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Initialize video subsystem
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        // Display error message
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        // Create window
        window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            // Display error message
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }
        else
        {
            // Create OpenGL context
            glContext = SDL_GL_CreateContext(window);

            if( glContext == NULL )
            {
                // Display error message
                printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
                return false;
            }
            else
            {
                // Initialize glew
                glewInit();
            }
        }
    }

	shader = Shader("shaders/SimpleShader.vert", "shaders/SimpleShader.frag");
	Armature arm = modelLoader.loadMS3dAnimation("Data/Skelitons/Test3.animation");
	CharacterBuilder charBuilder = CharacterBuilder();
	vector<Ms3dModel> models = charBuilder.loadCharacterFile("Data/Characters/Deathlord.char", &_textureHandler);
	_armatureHandler.addArmature(arm);
	for (int i = 0; i < models.size(); i++) {
		_modelHandler.addModel(Model(models[i], &_textureHandler));
	}
	//_modelHandler.getModel(0)->removeClothes(*_modelHandler.getModel(1));
	//Ms3dModel ms3dModel = modelLoader.loadMs3dModel("Data/Models/Weapons/Test.ms3d", &_textureHandler, false);
	//_modelHandler.addModel(Model(ms3dModel, &_textureHandler));
	Entity entity = Entity(vec3(0, 0, 0), &shader, &_modelHandler, arm);
	for (int i = 0; i < models.size(); i++) {
		entity.addModel(i);
	}
	entity.setAnimation("anm_headbang");
	_entities.push_back(entity);
	Camera camera = Camera(vec3(0.0f, 0.0f, -10.0f));
	glEnable(GL_DEPTH_TEST);
    //Used for delta time
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	// Game loop
	float speed = 2;
	int mx = 0;
	int my = 0;
	double fps = 0;
	int frames = 0;
	bool pause = false;
	while (!quit)
	{
		SDL_GetRelativeMouseState(&mx, &my);
		camera.setMousePosition(vec2(mx, my));
		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent) != 0)
		{
			if (sdlEvent.type == SDL_QUIT) {
				break;
			}
			if (keys[SDL_SCANCODE_W]) {
				camera.move(vec3(0.0F, 0.0F, speed*deltaTime));
			}
			if (keys[SDL_SCANCODE_S]) {
				camera.move(vec3(0.0F, 0.0F, -speed*deltaTime));
			}
			if (keys[SDL_SCANCODE_A]) {
				camera.move(vec3(-speed*deltaTime, 0.0F, 0.0F));
			}
			if (keys[SDL_SCANCODE_D]) {
				camera.move(vec3(speed*deltaTime, 0.0F, 0.0F));
			}
			if (keys[SDL_SCANCODE_Q]) {
				camera.move(vec3(0.0F, speed*deltaTime, 0.0F));
			}
			if (keys[SDL_SCANCODE_E]) {
				camera.move(vec3(0.0F, -speed*deltaTime, 0.0F));
			}
			if (keys[SDL_SCANCODE_1]) {
				entity.setAnimation("anm_base");
			}
			if (keys[SDL_SCANCODE_2]) {
				entity.setAnimation("anm_headbang");
			}
			if (keys[SDL_SCANCODE_3]) {
				entity.setAnimation("anm_punch1");
			}
			if (keys[SDL_SCANCODE_4]) {
				entity.setAnimation("anm_idle01");
			}
			if (keys[SDL_SCANCODE_ESCAPE]) {
				pause = !pause;
			}
		}

		// Set background color as cornflower blue
		glClearColor(0.39f, 0.58f, 0.93f, 1.f);
		// Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Render shit
		if (!pause) {
			vec2 center = vec2(WIDTH / 2, HEIGHT / 2);
			SDL_GetMouseState(&mx, &my);
			vec3 rot = vec3(0, ((center.x - mx) / 22)*deltaTime, ((center.y - my) / 22)*deltaTime);
			SDL_WarpMouseInWindow(window, (int)center.x, (int)center.y);
			camera.addRotation(rot);
		}
		//entity.setPosition(camera.getPosition());
		//entity.setRotation(camera.getRotation());
		entity.draw(camera, shader);
        // Update window with OpenGL rendering
        SDL_GL_SwapWindow(window);
		LAST = NOW;
	    NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency() );
		fps += deltaTime;
		frames++;
		if (fps >= 1) {
			cout << "FPS : " << frames << endl;
			fps = 0;
			frames = 0;
		}
    }

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}