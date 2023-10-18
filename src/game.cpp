#include "game.h"
#include "graphics.h"
#include "config.h"
#include <iostream>
using namespace std;

bool Game::checkCollision(Player* player, Projectile* pr, std::string blast)
{
	Disk plr = player->getCollisionHull();
	Disk prj = pr->getCollisionHull();
	Disk head = player->getHeadShot();
	
	prj.radius = blast == "normal" ? prj.radius : 130;
	float bdx = plr.cx - prj.cx;
	float bdy = plr.cy - prj.cy;
	float hdx = head.cx - prj.cx;
	float hdy = head.cy - prj.cy;
	bool body_distance = sqrt(bdx * bdx + bdy * bdy) < plr.radius + prj.radius;
	bool head_distance = sqrt(hdx * hdx + hdy * hdy) < head.radius + prj.radius;
	if (body_distance) {
		if (blast == "normal") {
			player->DrainLife(0.0065f);
		}
		else {
			player->DrainLife(0.2f);
		}
		return true;
	}
	else if (head_distance) {
		if (blast == "normal") {
			player->DrainLife(0.03f);
		}
		else {
			player->DrainLife(0.6f);
		}

		who_headshot = player->tag;
		animation_x = player->getX();
		animation_y = player->getY();
		animation_time = graphics::getGlobalTime() + 1500;
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::checkGhostCollision(Player* player, minnion* ghost)
{	
	if (!player) {
		return false;
	}

	Disk plr = player->getCollisionHull();
	Disk ghst = ghost->getCollisionHull_G();
	Disk head = player->getHeadShot();
	float bdx = plr.cx - ghst.cx;
	float bdy = plr.cy - ghst.cy;
	float hdx = head.cx - ghst.cx;
	float hdy = head.cy - ghst.cy;
	bool body_distance = sqrt(bdx * bdx + bdy * bdy) < plr.radius + ghst.radius;
	bool head_distance = sqrt(hdx * hdx + hdy * hdy) < head.radius + ghst.radius;
	if (body_distance || head_distance) {

		graphics::playSound(std::string(ASSET_PATH) + "Freeze - Sound Effect HD.mp3",0.15,false);
		return true;

	}
	else
	{
		return false;
	}

}

bool Game::checkCollisionWithFountain(Player* player)
{
	if (!player)
	{
		return false;
	}

	Disk plr = player->getCollisionHull();
	float dx = plr.cx - fountain_disk.cx;
	float dy = plr.cy - fountain_disk.cy;
	if (sqrt(dx * dx + dy * dy) < plr.radius + fountain_disk.radius)
	{
		player->DrainLife(-0.0003);
		return true;

	}
	else
	{	
		player->hp_up = false;
		return false;
	}
}

void Game::setupFountainProps()
{
	fountain_disk.cx = CANVAS_WIDTH / 2;
	fountain_disk.cy = CANVAS_HEIGHT - 85;
	fountain_disk.radius = 100;
}


void Game::update()
{
	if (graphics::getGlobalTime() > 1000 && !players_initialized && playable_state )
	{	
		graphics::playMusic(std::string(ASSET_PATH) + "Thunder Storm 02 by Fredrik Ekstrom - [Action Music].mp3", 0.2f, true, 3000);
		player1 = new Player();
		player2 = new Player();
		player1->tag = "Player1";
		player2->tag = "Player2";
		player1->setPosX();
		player2->setPosX();
		player1->player_choice = choicep1;
		player2->player_choice = choicep2;
		players_initialized = true;
		setupFountainProps();
	}

	if (player1 && player2 )
	{
		player1->update();
		player2->update();
		if (player1->getRemainingLife() == 0 || player2->getRemainingLife() == 0) {
			playable_state = false;
			endgame_state = true;
		    winner="PLAYER";
			winner += player1->getRemainingLife()?"1!!":"2!!";
			delete player1, player2, shots1, shots2,ghost1, ghost2;
			player1 = nullptr; 
			player2 = nullptr;
			ghost1 = nullptr;
			ghost2 = nullptr;
	
			shots1.clear(); shots1.shrink_to_fit();
			shots2.clear(); shots2.shrink_to_fit();
		}
	}
	
	if (playable_state && !ghost1)
	{
		ghost1 = new minnion();
		ghost1->tagm = 1;
		tag_time = graphics::getGlobalTime();

	}

	if (ghost1)
	{
		ghost1->update();
	}

		

	if (playable_state && !ghost2)
	{
		ghost2 = new minnion();
		ghost2->tagm = 3;
		tag_time2 = graphics::getGlobalTime();
		
	}

	if (ghost2)
		ghost2->update();

	if (ghost1 && graphics::getGlobalTime() >= tag_time + 15000) {
		
		tag_count = tag_count % 2 + 1;
		ghost1->tagm = tag_count;
		tag_time = graphics::getGlobalTime();
	}

	if (ghost2 && graphics::getGlobalTime() >= tag_time2 + 15000) {

		tag_count2 = tag_count2 % 4 + 1;
		ghost2->tagm = tag_count2;
		tag_time2 = graphics::getGlobalTime();
	}




	//player1 shots
	if (playable_state && graphics::getKeyState(graphics::SCANCODE_SPACE) && player1 && graphics::getGlobalTime() >= nextAttackTimeP1)
	{
		shots1.push_back(new Projectile(player1->getX() - 10, player1->getY(), pNp1, player1->tag, ps1, false));
	}

	if (!shots1.empty())
	{
		for (int i = 0; i < shots1.size(); i++)
		{
			shots1[i]->update();
			if (shots1[i]->outOfBorders() || checkCollision(player2, shots1[i], "normal"))
			{
				shots1.erase(shots1.begin() + i);
			}
		}
		nextAttackTimeP1 = graphics::getGlobalTime() + 1 / attackRate;
	}

	//player2 shots
	if (playable_state && graphics::getKeyState(graphics::SCANCODE_RCTRL) && player2 && graphics::getGlobalTime() >= nextAttackTimeP2)
	{
		shots2.push_back(new Projectile(player2->getX() + 10, player2->getY(), pNp2, player2->tag, ps2, false));
	}

	if (!shots2.empty())
	{
		for (int i = 0; i < shots2.size(); i++)
		{
			shots2[i]->update();
			if (shots2[i]->outOfBorders() || checkCollision(player1, shots2[i], "normal"))
			{
				shots2.erase(shots2.begin() + i);
			}
		}
		nextAttackTimeP2 = graphics::getGlobalTime() + 1 / attackRate;
	}
	//ultimate attacks / player1
	if (playable_state && graphics::getKeyState(graphics::SCANCODE_F) && player1 && graphics::getGlobalTime() >= nextAttackTimeP1_U) {

		ultiv1.push_back(new Projectile(player1->getX() - 10, player1->getY(), ulti1, player1->tag, ulti_sound1, true));
	}
	if (!ultiv1.empty())
	{
		for (int i = 0; i < ultiv1.size(); i++)
		{
			ultiv1[i]->update();
			if (checkCollision(player2, ultiv1[i], "ulti") || ultiv1[i]->outOfBorders()) {
				ultiv1.erase(ultiv1.begin() + i);
			}

		}
		nextAttackTimeP1_U = graphics::getGlobalTime() + 7000;
	}
	//ultimate attacks / player2
	if (playable_state && graphics::getKeyState(graphics::SCANCODE_RSHIFT) && player2 && graphics::getGlobalTime() >= nextAttackTimeP2_U) {

		ultiv2.push_back(new Projectile(player2->getX() + 10, player2->getY(), ulti2, player2->tag, ulti_sound2, true));
	}
	if (!ultiv2.empty())
	{
		for (int i = 0; i < ultiv2.size(); i++)
		{
			ultiv2[i]->update();
			if (checkCollision(player1, ultiv2[i], "ulti") || ultiv2[i]->outOfBorders()) {
				ultiv2.erase(ultiv2.begin() + i);
			}

		}
		nextAttackTimeP2_U = graphics::getGlobalTime() + 7000;
	}
	
	//fountain events

	if (checkCollisionWithFountain(player1)){
		player1->hp_up = true;
	}
	if (checkCollisionWithFountain(player2)) {
		player2->hp_up = true;
	}




	if(checkGhostCollision(player1, ghost1)|| checkGhostCollision(player1, ghost2)) {
		player1->freeze = true;
		un_freezed1 = graphics::getGlobalTime();
	}
	if (checkGhostCollision(player2, ghost1) || checkGhostCollision(player2, ghost2)) {
		player2->freeze = true;
		un_freezed2 = graphics::getGlobalTime();
	}

	if (player1 && graphics::getGlobalTime() >= un_freezed1 + 1500) {
		player1->freeze = false;
	}
	if (player2 && graphics::getGlobalTime() >= un_freezed2 + 1500){
		player2->freeze = false;
	}
}



void Game::draw()
{
	 
	graphics::Brush br;
	if (loginscreen_state || menu_state1 || menu_state2 || arena_selectionState || control_state || pregame_state)
	{
		br.texture = std::string(ASSET_PATH) + "background.png";
		br.outline_opacity = 0;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
	}
	//startup screen
	if (loginscreen_state)
	{
		std::string s1 = "FIGHTERS COLLIDE!";
		std::string s2 = "press  Space  to  continue";

		graphics::setFont(std::string(ASSET_PATH) + "Shoguns Clan.ttf");
		graphics::drawText(CANVAS_WIDTH / 9, 100, 75, s1, br);
		graphics::drawText(CANVAS_WIDTH / 6, 370, 45, s2, br);
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			loginscreen_state = false;
			menu_state1 = true;
		}
	}
	//player1 character selection
	if (menu_state1)
	{

		br.outline_opacity = .0f;
		br.fill_opacity = 1.f;


		br.texture = std::string(ASSET_PATH) + "wizard.png";
		graphics::drawRect(850, 150, 75, 75, br);
		br.texture = std::string(ASSET_PATH) + "viking.png";
		graphics::drawRect(850, 240, 75, 75, br);
		br.texture = std::string(ASSET_PATH) + "ninja.png";
		graphics::drawRect(850, 350, 75, 75, br);
		br.texture = std::string(ASSET_PATH) + "DOCTOR.png";
		graphics::drawRect(850, 450, 75, 75, br);

		std::string s1 = "CHOOSE A CHARACTER";
		std::string s1snd = "for player 1";
		std::string s2 = "press 1 for wizard";
		std::string s3 = "press 2 for viking";
		std::string s4 = "press 3 for ninja";
		std::string s5 = "press 4 for doctor";

		graphics::setFont(std::string(ASSET_PATH) + "SEVESBRG.TTF");
		graphics::drawText(25, 80, 75, s1, br);
		graphics::drawText(340, 130, 55, s1snd, br);
		graphics::drawText(CANVAS_WIDTH / 6, 170, 45, s2, br);
		graphics::drawText(CANVAS_WIDTH / 6, 270, 45, s3, br);
		graphics::drawText(CANVAS_WIDTH / 6, 370, 45, s4, br);
		graphics::drawText(CANVAS_WIDTH / 6, 470, 45, s5, br);
		if (graphics::getKeyState(graphics::SCANCODE_1)) {
			choicep1 = 1;
			choice_given = true;
			pNp1 = "fire.png";
			ulti1 = "wizard_ulti.png";
			ulti_sound1 = "W_sound.mp3";
			ps1 = "Fireball.mp3";
			p1texture = "wizard.png";
		}
		else if (graphics::getKeyState(graphics::SCANCODE_2)) {
			choicep1 = 2;
			choice_given = true;
			pNp1 = "tomahawk.png";
			ulti1 = "viking_ulti.png";
			ps1 = "Arrow.mp3";
			p1texture = "viking.png";
			ulti_sound1 = "V_sound.mp3";
		}
		else if (graphics::getKeyState(graphics::SCANCODE_3)) {
			choicep1 = 3;
			choice_given = true;
			pNp1 = "shuriken.png";
			ulti1 = "ninja_ulti.png";
			ps1 = "Ninja Star.mp3";
			p1texture = "ninja.png";
			ulti_sound1 = "N_sound.mp3";
		}
		else if (graphics::getKeyState(graphics::SCANCODE_4)) {
			choicep1 = 4;
			choice_given = true;
			pNp1 = "covid.png";
			ulti1 = "doctor_ulti.png";
			ps1 = "whalesquish.mp3";
			p1texture = "DOCTOR.png";
			ulti_sound1 = "D_sound.mp3";
		}
		if (choice_given) {
			menu_state1 = false;
			menu_state2 = true;
		}
	}
	//player2 character selection
	if (menu_state2)
	{

		br.texture = std::string(ASSET_PATH) + "wizard.png";
		graphics::drawRect(850, 150, 75, 75, br);
		br.texture = std::string(ASSET_PATH) + "viking.png";
		graphics::drawRect(850, 240, 75, 75, br);
		br.texture = std::string(ASSET_PATH) + "ninja.png";
		graphics::drawRect(850, 350, 75, 75, br);
		br.texture = std::string(ASSET_PATH) + "DOCTOR.png";
		graphics::drawRect(850, 450, 75, 75, br);


		std::string s1 = "CHOOSE A CHARACTER";
		std::string s1snd = "for player 2";
		std::string s2 = "press A for wizard";
		std::string s3 = "press B for viking";
		std::string s4 = "press C for ninja";
		std::string s5 = "press D for doctor";

		graphics::setFont(std::string(ASSET_PATH) + "SEVESBRG.TTF");
		graphics::drawText(25, 80, 75, s1, br);
		graphics::drawText(340, 120, 55, s1snd, br);
		graphics::drawText(CANVAS_WIDTH / 6, 170, 45, s2, br);
		graphics::drawText(CANVAS_WIDTH / 6, 270, 45, s3, br);
		graphics::drawText(CANVAS_WIDTH / 6, 370, 45, s4, br);
		graphics::drawText(CANVAS_WIDTH / 6, 470, 45, s5, br);
		if (graphics::getKeyState(graphics::SCANCODE_A)) {
			choicep2 = 1;
			choice_given2 = true;
			pNp2 = "fire.png";
			ulti2 = "wizard_ulti.png";
			ps2 = "Fireball.mp3";
			p2texture = "wizard.png";
			ulti_sound2 = "W_sound.mp3";
		}
		else if (graphics::getKeyState(graphics::SCANCODE_B)) {
			choicep2 = 2;
			choice_given2 = true;
			pNp2 = "tomahawk.png";
			ulti2 = "viking_ulti.png";
			ps2 = "Arrow.mp3";
			p2texture = "viking.png";
			ulti_sound2 = "V_sound.mp3";
		}
		else if (graphics::getKeyState(graphics::SCANCODE_C)) {
			choicep2 = 3;
			choice_given2 = true;
			pNp2 = "shuriken.png";
			ulti2 = "ninja_ulti.png";
			ps2 = "Ninja Star.mp3";
			p2texture = "ninja.png";
			ulti_sound2 = "N_sound.mp3";
		}
		else if (graphics::getKeyState(graphics::SCANCODE_D)) {
			choicep2 = 4;
			choice_given2 = true;
			pNp2 = "covid.png";
			ulti2 = "doctor_ulti.png";
			ps2 = "whalesquish.mp3";
			p2texture = "DOCTOR.png";
			ulti_sound2 = "D_sound.mp3";
		}
		if (choice_given2) {
			menu_state2 = false;
			arena_selectionState = true;
		}

	}
	if (arena_selectionState)
	{
		br.texture = std::string(ASSET_PATH) + "ClassicArena.png";
		graphics::drawRect(900, 150, 90, 90, br);
		br.texture = std::string(ASSET_PATH) + "FireArena.png";
		graphics::drawRect(900, 250, 90, 90, br);
		br.texture = std::string(ASSET_PATH) + "MoonArena.png";
		graphics::drawRect(900, 350, 90, 90, br);
		br.texture = std::string(ASSET_PATH) + "SnowArena.png";
		graphics::drawRect(900, 450, 90, 90, br);

		std::string s1 = "CHOOSE ARENA";
		std::string s2 = "press 1 for classic arena";
		std::string s3 = "press 2 for fire arena";
		std::string s4 = "press 3 for moonlight arena";
		std::string s5 = "press 4 for snow arena";
		graphics::setFont(std::string(ASSET_PATH) + "SEVESBRG.TTF");
		graphics::drawText(CANVAS_WIDTH / 9, 80, 75, s1, br);

		graphics::drawText(20, 170, 45, s2, br);
		graphics::drawText(20, 270, 45, s3, br);
		graphics::drawText(20, 370, 45, s4, br);
		graphics::drawText(20, 470, 45, s5, br);

		if (graphics::getKeyState(graphics::SCANCODE_1)) {
			arena = "classicArena.png";
			arenaSelected = true;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_2)) {
			arena = "FireArena.png";
			arenaSelected = true;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_3)) {
			arena = "MoonArena.png";
			arenaSelected = true;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_4)) {
			arena = "SnowArena.png";
			arenaSelected = true;
		}
		if (arenaSelected) {
			arena_selectionState = false;
			control_state = true;
		}

	}
	if (control_state) {
		br.texture = std::string(ASSET_PATH) + "scroll.png";
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 1050, 510, br);

		br.texture = std::string(ASSET_PATH) + "instructions.png";
		graphics::drawRect(250, 210, 350, 400, br);

		br.texture = std::string(ASSET_PATH) + "tip.png";
		graphics::setOrientation(-4);
		graphics::drawRect(230, 380, 330, 90, br);
		graphics::resetPose();


		br.texture = std::string(ASSET_PATH) + "extra_info.png";
		graphics::drawRect(720, 230, 350, 400, br);

		br.texture = std::string(ASSET_PATH) + "extra_tip.png";
		graphics::setOrientation(-2);
		graphics::drawRect(840, 55, 320, 90, br);
		graphics::resetPose();




		br.texture = std::string(ASSET_PATH) + "space.png";
		graphics::drawRect(500, 470, 600, 70, br);

		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			control_state = false;
			pregame_state = true;
		}

	}
	//versus screen
	if (pregame_state)
	{
		br.texture = std::string(ASSET_PATH) + "vs_screen.png";
		br.outline_opacity = 0;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		if (!sound_played)
		{
			graphics::playSound(std::string(ASSET_PATH) + "Boxing Bell Sound Effect (mp3cut.net).mp3", 1.2f, false);
			sound_played = true;
		}

		br.texture = std::string(ASSET_PATH) + p1texture;
		graphics::drawRect(240, 250, 300, 300, br);
		br.texture = std::string(ASSET_PATH) + p2texture;
		graphics::drawRect(750, 250, 300, 300, br);

		std::string s1 = "FIGHTERS COLLIDE!";
		std::string s2 = " press     E     to    enter    arena";
		graphics::drawText(50, 60, 85, s1, br);
		graphics::setFont(std::string(ASSET_PATH) + "Shoguns Clan.ttf");

		graphics::drawText(CANVAS_WIDTH / 14, 470, 50, s2, br);
		if (graphics::getKeyState(graphics::SCANCODE_E)) {
			pregame_state = false;
			playable_state = true;
		}
	}

	//draw actual gameplay
	if (playable_state)
	{
		//draw background
		br.texture = std::string(ASSET_PATH) + arena;
		br.outline_opacity = 0;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		//draw player1, player2
		if (player1 && player2)
		{
			player1->draw();
			player2->draw();
		}
		//draw player1's projectiles
		if (!shots1.empty()) {
			for (auto it : shots1)
			{
				it->draw();
			}
		}
		//draw player2's projectiles
		if (!shots2.empty()) {
			for (auto it : shots2)
			{
				it->draw();
			}
		}
		if (!ultiv1.empty()) {
			for (auto it : ultiv1)
			{
				it->draw();
			}
		}
		if (!ultiv2.empty()) {
			for (auto it : ultiv2)
			{
				it->draw();
			}
		}
		//draw head
		if (ghost1)
			ghost1->draw();
		if (ghost2)
			ghost2->draw();
		
		if (graphics::getGlobalTime() <= animation_time)
		{
			if (who_headshot == "Player1")
			{
				br.outline_opacity = 0.0f;
				br.texture = std::string(ASSET_PATH) + "headshot.png";
				br.fill_opacity = 1;
				graphics::drawRect(animation_x, animation_y - 60, 250, 65, br);
				graphics::playSound(std::string(ASSET_PATH) + "Headshot - Sound Effect [Perfect Cut].mp3", 0.2f, false);
			}

			if (who_headshot == "Player2")
			{
				br.outline_opacity = 0.0f;
				br.texture = std::string(ASSET_PATH) + "headshot.png";
				br.fill_opacity = 1;
				graphics::drawRect(animation_x, animation_y - 60, 250, 65, br);
				graphics::playSound(std::string(ASSET_PATH) + "Headshot - Sound Effect [Perfect Cut].mp3", 0.2f, false);
			}

		}
		//life regeneration fountain
		br.outline_opacity = .0f;
		br.fill_opacity = 1.f;
		br.texture = std::string(ASSET_PATH) + "fountain.png";
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT + 10, 200, 300, br);

		//life bars
		float playerLife1 = player1 ? player1->getRemainingLife() : 0.0f;
		float playerLife2 = player2 ? player2->getRemainingLife() : 0.0f;
		br.texture = "";
		br.outline_opacity = 0.0f;
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.15f;
		br.fill_color[2] = 0.0f;
		br.fill_secondary_color[0] = 0.2f;
		br.fill_secondary_color[1] = 1.0f;
		br.fill_secondary_color[2] = 0.0f;
		br.gradient = true;
		br.gradient_dir_u = 1.0f;
		br.gradient_dir_v = 0.0f;

		graphics::drawRect(75 + playerLife1 * 200 / 2, 30, playerLife1 * 200, 30, br);
		graphics::drawRect(730 + playerLife2 * 200 / 2, 30, playerLife2 * 200, 30, br);

		br.outline_opacity = 1.0f;
		br.gradient = false;
		br.fill_opacity = 0.0;
		graphics::drawRect(75 + 100, 30, 200, 30, br);
		graphics::drawRect(730 + 100, 30, 200, 30, br);
	}

	//end game
	if (endgame_state == true)
	{
		if (!sound_played2) {
			graphics::playSound(std::string(ASSET_PATH) + "Game Over sound effect.mp3", 1.2f, false);
			sound_played2 = true;
		}
		graphics::setFont(std::string(ASSET_PATH) + "Shoguns Clan.ttf");
		br.texture = std::string(ASSET_PATH) + "endscreen.png";
		br.outline_opacity = 0;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
		string s1 = "GAME  OVER";
		string s2 = "press   'esc'   to     EXIT    ,  OR";
		string s3 = "press   'R'   to   play   AGAIN ";
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 0.0f;
		graphics::drawText(130, 155, 120, s1, br);
		graphics::drawText(200, 330, 55, "WINNER   IS   " + winner, br);
		br.fill_color[0] = 0.6f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 1.0f;
		graphics::drawText(45, 440, 35, s2, br);
		graphics::drawText(45, 490, 35, s3, br);
		if (graphics::getKeyState(graphics::SCANCODE_R)) {
			endgame_state = false;
			loginscreen_state = true;
			arenaSelected = false;
			choice_given = false;
			choice_given2 = false;
			choicep1 = 0;
			choicep2 = 0;
			players_initialized = false;
			sound_played = false;
			graphics::playMusic(std::string(ASSET_PATH) + "Last Strike for Glory.mp3", 0.2f, true, 3000);
		}
		if (graphics::getKeyState(graphics::SCANCODE_ESCAPE)) {
			exit(0);
		}
	}
}


void Game::init()
{
	graphics::playMusic(std::string(ASSET_PATH) + "Last Strike for Glory.mp3",0.2f,true,3000);
}

Game::~Game()
{
	if (player1)
		delete player1;
	if (player2)
		delete player2;
	shots1.clear();
	shots1.shrink_to_fit();
	shots2.clear();
	shots2.shrink_to_fit();

}

Game::Game()
{

}