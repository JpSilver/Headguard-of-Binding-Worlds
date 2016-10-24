#ifndef CHARACTERBUILDER_H
#define CHARACTERBUILDER_H

#include <iostream>
#include "ms3dModelLoader.h"
#include "Model.h"

class CharacterBuilder {
public:
	CharacterBuilder();
	vector<Ms3dModel>  loadCharacterFile(string, TextureHandler*);
private:
};

#endif //CHARACTERBUILDER_H