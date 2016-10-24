#include "TextureHandler.h"

Texture *TextureHandler::addTexture(Texture texture) {
	textures.push_back(texture);
	return &textures.back();
};

int TextureHandler::getSize() {
	return this->textures.size();
}

Texture *TextureHandler::getTexture(string name) {
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i].getName() == name) {
			return &textures[i];
		}
	}
	return nullptr;
}
Texture * TextureHandler::getTexture(int index)
{
	return &textures[index];
};