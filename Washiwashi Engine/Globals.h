#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>

#define WASHI_LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#define LIBRARY_PATH "Engine/Library"
#define FOLDERS_PATH "Engine/Library/Folders/"
#define MESHES_PATH "Engine/Library/Meshes/"
#define MATERIALS_PATH "Engine/Library/Materials/"
#define TEXTURES_PATH "Library/Textures/"
#define MODELS_PATH "Engine/Library/Models/"
#define ANIMATIONS_PATH "Engine/Library/Animations/"
#define BONES_PATH "Engine/Library/Bones/"
#define PARTICLES_PATH "Engine/Library/ParticleSystems/"
#define SHADERS_PATH "Engine/Library/Shaders/"
#define SCENES_PATH "Engine/Library/Scenes/"

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define INVALID_OGL_VALUE 0xffffffff
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals |  aiProcess_JoinIdenticalVertices )

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned int uint;

// Configuration -----------
#define SCREEN_SIZE 1
#define VSYNC true
#define TITLE "Washiwashi Engine"
#define ORGANIZATION "UPC CITM"

// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }