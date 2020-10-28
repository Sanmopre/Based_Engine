#pragma once

#include "Globals.h"

class GameObject;

class Item
{
	GameObject* object;
};

class Tree
{
	std::vector<Item*> items;
};
