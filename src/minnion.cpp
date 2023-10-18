#include "minnion.h"
#include "config.h"
#include <random>

void minnion::update()
{
	if (tagm == 1 || tagm ==3) { 
		posm_x -= speed * graphics::getDeltaTime();
		if (posm_x < -size) {
			init();
		}
	}
	else  {
		posm_x += speed * graphics::getDeltaTime();
		if (posm_x > CANVAS_WIDTH + size) {
			init();
		}
	}
	rotation += 0.05f * graphics::getDeltaTime();
	rotation = fmodf(rotation, 360);

}

void minnion::draw()
{	

	brush.fill_opacity = 0.65f;
	if (tagm == 1) {
		graphics::setOrientation(rotation);
		brush.texture = std::string(ASSET_PATH) + "head.png";
		graphics::drawRect(posm_x, posm_y, size, size, brush);
		graphics::resetPose();
	}
	if (tagm == 2) {
		graphics::setOrientation(rotation);
		brush.texture = std::string(ASSET_PATH) + "samurai.png";
		graphics::drawRect(posm_x, posm_y, size, size, brush);
		graphics::resetPose();
	}
	if (tagm == 3) {
		graphics::setOrientation(rotation);
		brush.texture = std::string(ASSET_PATH) + "witch.png";
		graphics::drawRect(posm_x, posm_y, size, size, brush);
		graphics::resetPose();
	}
	if (tagm == 4) {
		graphics::setOrientation(rotation);
		brush.texture = std::string(ASSET_PATH) + "warrior.png";
		graphics::drawRect(posm_x, posm_y, size, size, brush);
		graphics::resetPose();
	}
}

void minnion::init()
{	
	if (tagm == 1 ) { posm_x = CANVAS_WIDTH + 800; speed = 0.06;
	}
	else if (tagm == 2) {
		posm_x = -350; speed = 0.06; 
	}
	else  if (tagm==3)
	{
		posm_x = CANVAS_WIDTH + 2000; speed = 0.09;
	}
	else {
		posm_x = -1500; speed = 0.09;
	}
	posm_y = CANVAS_HEIGHT *rand()/(float)RAND_MAX;
	size = 50 + 100* rand() / (float)RAND_MAX;
	rotation= 180* rand() / (float)RAND_MAX;
	brush.outline_opacity = 0.0f;
	
}

Disk minnion::getCollisionHull_G()
{
	Disk disk;
	disk.cx = posm_x;
	disk.cy = posm_y + 17;
	disk.radius = 35.0f;
	return disk;
}

minnion::minnion()
{
	init();
}

minnion::~minnion()
{
}
