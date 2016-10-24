#include "ArmatureHandler.h"

Armature *ArmatureHandler::addArmature(Armature model) {
	this->armatures.push_back(model);
	return &armatures.back();
};

Armature *ArmatureHandler::getArmature(string name) {
	for (int i = 0; i < armatures.size(); i++) {
		if (armatures[i].getName() == name) {
			return &armatures[i];
		}
	}
	return NULL;
}