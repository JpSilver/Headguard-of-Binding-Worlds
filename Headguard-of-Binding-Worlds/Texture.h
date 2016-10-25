#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>
#include <string>
#include <iostream>
#include "SOIL.h"


using std::cout;
using std::endl;

using std::string;

class Texture {
public:
	Texture();
	Texture(string texturePath);
	GLuint getTextureID();
	string getName();
	
private:
	GLuint textureID;
	string name;
};

#endif //TEXTURE_H