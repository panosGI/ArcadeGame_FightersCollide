#include "player.h"
#include "graphics.h"

float Player::getX() {
	return posX;
}

float Player::getY() {
	return posY;
}

void Player::setPosX()
{
	posX = (tag == "Player1") ? CANVAS_WIDTH / 2 - 300 : CANVAS_WIDTH / 2 + 300;
}

Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = posX;
	disk.cy = posY+17;
	disk.radius = 35.0f;
	return disk;
}

Disk Player::getHeadShot() const
{
	Disk disk;
	disk.cx = posX;
	disk.cy = posY-30;
	disk.radius = 15.0f;
	return disk;
}

void Player::update()
{
	if (tag == "Player1")
	{	
		if (graphics::getKeyState(graphics::SCANCODE_A))
		{	
			if (freeze) {
				posX = posX;
			}
			else {
				posX -= speed * graphics::getDeltaTime() / 10.0f;
			}
		}
		if (graphics::getKeyState(graphics::SCANCODE_D))
		{	
			if (freeze) { 
				posX = posX;
			}
			else {
				posX += speed * graphics::getDeltaTime() / 10.0f;
			}
		}
		if (graphics::getKeyState(graphics::SCANCODE_W))
		{	
			if (freeze) { 
				posY = posY;
			}
			else {
				posY -= speed * graphics::getDeltaTime() / 10.0f;
			}
		}
		if (graphics::getKeyState(graphics::SCANCODE_S))
		{	
			if (freeze) {
				posY = posY;
			}
			else {
				posY += speed * graphics::getDeltaTime() / 10.0f;
			}
		}
	}
	else if (tag == "Player2")
	{
		if (graphics::getKeyState(graphics::SCANCODE_LEFT))
		{
			if (freeze) {
				posX = posX; 
			}
			else {
				posX -= speed * graphics::getDeltaTime() / 10.0f;
			}
		}
		if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
		{	
			if (freeze) { 
				posX = posX; 
			}
			else {
				posX += speed * graphics::getDeltaTime() / 10.0f;
			}
		}
		if (graphics::getKeyState(graphics::SCANCODE_UP))
		{	
			if (freeze) {
				posY = posY;
			}
			else {
				posY -= speed * graphics::getDeltaTime() / 10.0f;
			}
		}
		if (graphics::getKeyState(graphics::SCANCODE_DOWN))
		{	
			if (freeze) {
				posY = posY;
			}
			else {
				posY += speed * graphics::getDeltaTime() / 10.0f;
			}
		}
	}
	//oria paikth
	if (posX < 0) { posX = 0; }
	if (posX > CANVAS_WIDTH) { posX = CANVAS_WIDTH; }
	if (posY < 0) { posY = 0; }
	if (posY > CANVAS_HEIGHT) { posY = CANVAS_HEIGHT; }
}

void Player::draw()
{		
	

		graphics::Brush br;
		br.outline_opacity = .0f;
		br.fill_opacity = 1.f;
		if (player_choice == 1)
		{
			br.texture = std::string(ASSET_PATH) + "wizard.png";
			graphics::drawRect(posX, posY, 100, 100, br);		
		}
		else if (player_choice == 2)
		{
			br.texture = std::string(ASSET_PATH) + "viking.png";
			graphics::drawRect(posX, posY, 100, 100, br);
		}
		else if (player_choice == 3)
		{

			br.texture = std::string(ASSET_PATH) + "ninja.png";
			graphics::drawRect(posX, posY, 100, 100, br);
		}
		else if (player_choice == 4)
		{
			br.texture = std::string(ASSET_PATH) + "DOCTOR.png";
			graphics::drawRect(posX, posY, 100, 100, br);
		}
		if (freeze) {
			br.outline_opacity = .0f;
			br.fill_opacity = 1.f;
			br.texture = std::string(ASSET_PATH) + "frost.png";
			graphics::drawRect(posX, posY, 100, 100, br);

		}
		if (hp_up) {
			br.outline_opacity = .0f;
			br.fill_opacity = 1.f;
			br.texture = std::string(ASSET_PATH) + "health.png";
			graphics::drawRect(posX, posY, 70, 70, br);

		}


		//debugging info
		/*br.outline_opacity = 1.f;
		br.texture = "";
		br.fill_color[0] = .3f;
		br.fill_color[1] = 1.f;
		br.fill_color[2] = .3f;
		br.fill_opacity = .3f;
		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);

		br.fill_color[0] = 1.f;
		br.fill_color[1] = .2f;
		br.fill_color[2] = .3f;
		br.fill_opacity = .3f;
		hull = getHeadShot();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);*/
	
}


void Player::init()
{

}