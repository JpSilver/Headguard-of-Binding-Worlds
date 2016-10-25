#include "EntityHandler.h"

Entity * EntityHandler::addEntity(Entity entity)
{
	entities.push_back(entity);
	return &entities[entities.size()];
}

Entity * EntityHandler::getEntity(string name)
{
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i].getName() == name) {
			return &entities[i];
		}
	}
	return nullptr;
}

vector<Entity> EntityHandler::getEntities()
{
	return entities;
}

void EntityHandler::drawEntities(Camera cam) {
	for (int i = 0; i < entities.size(); i++) {
		//entities[i].draw(cam);
	}
}