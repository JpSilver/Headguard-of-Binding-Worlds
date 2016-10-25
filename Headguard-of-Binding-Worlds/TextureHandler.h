#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include "Texture.h"
#include <vector>

using std::vector;


class TextureHandler {
public:
	Texture *addTexture(Texture);
	Texture *getTexture(string name);
	Texture *getTexture(int index);
	int getSize();
private:
	vector<Texture> textures;
};

#endif //Model Handler