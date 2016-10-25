#ifndef ARMATUREHANDLER_H
#define ARMATUREHANDLER_H

#include "Armature.h"
#include <vector>

using std::vector;

class ArmatureHandler {
public:
	Armature *addArmature(Armature);
	Armature *getArmature(string);
private:
	vector<Armature> armatures;
};

#endif //ARMATUREHANDLER_H