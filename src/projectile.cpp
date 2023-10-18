#include "projectile.h"
#include "graphics.h"

void Projectile::update()
{
	if (tagP == "Player1")
	{
		pospx += speed * graphics::getDeltaTime();
	}
	else if (tagP == "Player2")
	{
		pospx -= speed * graphics::getDeltaTime();
	}
}

void Projectile::draw()
{
	graphics::Brush br;
	br.outline_opacity = .0f;
	br.fill_opacity = 1.f;
	br.texture = std::string(ASSET_PATH) + projectileFileName;
	graphics::drawRect(pospx, pospy, size, size, br);

	//debugging info
	/*br.outline_opacity = 1.f;
	br.texture = "";
	br.fill_color[0] = .3f;
	br.fill_color[1] = 1.f;
	br.fill_color[2] = .3f;
	br.fill_opacity = .3f;
	Disk hull = getCollisionHull();
	graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);*/
}

void Projectile::init()
{

}

Projectile::Projectile(float x, float y, std::string projectileName, std::string tag, std::string projectileSound, bool is_ulti)
{
	speed = 1.7f;
	size = 60.f;
	if (is_ulti) {
		speed = .3f;
		size = 250.f;
	}
	pospx = x;
	pospy = y;
	projectileFileName = projectileName;
	tagP = tag;
	graphics::playSound(std::string(ASSET_PATH) + projectileSound, .4f, false);
}

Projectile::Projectile()
{

}

Projectile::~Projectile()
{
	
}

bool Projectile::outOfBorders()
{
	if (pospx < 0 || pospx > CANVAS_WIDTH)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Disk Projectile::getCollisionHull() const
{
	Disk disk;
	disk.cx = pospx;
	disk.cy = pospy;
	disk.radius = size/3;
	return disk;
}