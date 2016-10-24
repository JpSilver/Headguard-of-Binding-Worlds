#include "ModelHandler.h"

Model *ModelHandler::addModel(Model model) {
	this->models.push_back(model);
	return &this->models.back();
};

Model *ModelHandler::getModel(string name) {
	for (int i = 0; i < models.size(); i++) {
		if (models[i].getName() == name) {
			return &models[i];
		}
	}
	return nullptr;
}

Model * ModelHandler::getModel(int index)
{
	return &models[index];
}

int ModelHandler::getSize()
{
	return models.size();
}
