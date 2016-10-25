#ifndef MODELHANDLER_H
#define MODELHANDLER_H

#include <vector>
#include "Model.h"

using std::vector;


class ModelHandler{
public:
	ModelHandler(){};
	Model *addModel(Model model);
	Model *getModel(string name);
	Model *getModel(int index);
	int getSize();
private:
	vector<Model> models;
};

#endif //MODELHANDLER_H