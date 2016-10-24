#ifndef ENTITYHANDLER_H
#define ENTITYHANDLER_H

#include "Entity.h"


class EntityHandler {
public:
	EntityHandler() {};
	Entity *addEntity(Entity);
	Entity *getEntity(string);
	vector<Entity> getEntities();
	void drawEntities(Camera);
private:
	vector<Entity> entities;
};

#endif //ARMATUREHANDLER_H