#pragma once
#include"gameobject.h"
#include "graphics.h"

class minnion :  GameObject {
	float posm_x, posm_y;
	float size;
	float speed;
	float rotation;
	graphics::Brush brush;
public:
	int tagm;
	void update() override;
	void draw() override;
	 void init() override;
	 Disk getCollisionHull_G() ;
	 minnion();
	 ~minnion();
};