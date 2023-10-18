#pragma once
#include "gameobject.h"
#include "config.h"
#include "player.h"
#include <string>

class Projectile :public GameObject, public Collidable {

	float speed, pospx, pospy, size;
	std::string projectileFileName, tagP;
public:
	void update() override;
	void draw() override;
	void init() override;
	Projectile();
	Projectile(float x,float y, std::string projectileName, std::string tag, std::string projectileSound,bool is_ulti);
	~Projectile();
	bool outOfBorders();
	Disk getCollisionHull() const override;
	Disk getCollisionHull_U() ;

	float getx() { return pospx; }
};