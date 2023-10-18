#pragma once
#include "tools.h"


class Collidable
{
public:
	virtual Disk getCollisionHull() const = 0;
};

class GameObject //interface
{
public:
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void init() = 0;
};