#pragma once
#include "gameobject.h"
#include "config.h"
#include <string>

class Player :public GameObject, public Collidable
{
public:
	std::string tag;
	int player_choice;
	float posX;
	float posY = CANVAS_HEIGHT / 2;
	float speed = 6.2;
	float life = 1.0f;
	bool freeze = false;
	bool hp_up = false;
public:
	void update() override;
	void draw() override;
	void init() override;
	float getX();
	float getY();
	void setPosX();
	Disk getCollisionHull() const override;
	Disk getHeadShot() const;
	float getRemainingLife() const { return life; }
	void DrainLife(float amount) { life = std::max<float>(0.0f, std::min(1.f, life - amount)); }
};