#include "Texture.h"

Texture::Texture(string texture) {
	
	this->textureID = SOIL_load_OGL_texture(texture.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	/* check for an error during the load process */
	if (0 == this->textureID)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	
};

Texture::Texture() {
	textureID = -1;
}

GLuint Texture::getTextureID() {
	return this->textureID;
}

string Texture::getName()
{
	return this->name;
}
