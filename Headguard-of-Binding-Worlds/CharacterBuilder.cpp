#include "CharacterBuilder.h"

CharacterBuilder::CharacterBuilder()
{
}

vector<Ms3dModel> CharacterBuilder::loadCharacterFile(string filename, TextureHandler *textureHandler)
{
	ifstream file(filename.c_str());
	string line;
	vector<Ms3dModel> models;
	ms3dModelLoader loader = ms3dModelLoader();
	while (getline(file, line)) {
		models.push_back(loader.loadMs3dModel(string("Data/") + line, textureHandler, false));
	};
	return models;
};
