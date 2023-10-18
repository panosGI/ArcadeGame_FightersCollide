#pragma once
#include "player.h"
#include "projectile.h"
#include <string>
#include <vector> 
#include <iterator>
#include "minnion.h"


class Game  
{
	//entities
	Player * player1 = nullptr;
	Player * player2 = nullptr;
	minnion* ghost1 = nullptr;
	minnion* ghost2 = nullptr;
	float tag_time;
	float tag_time2;
	int tag_count = 0;
	int tag_count2 = 2;
	std::vector<Projectile*> shots1, shots2, ultiv1, ultiv2;
	Disk fountain_disk;

	//game controllers
	float un_freezed1 = 0.0f;
	float un_freezed2 = 0.0f;
	float nextAttackTimeP1 = 0.f;
	float nextAttackTimeP2 = 0.f;
	float nextAttackTimeP1_U = 0.f;
	float nextAttackTimeP2_U = 0.f;
	float attackRate = .02f;
	float animation_time, animation_x, animation_y;
	std::string pNp1, pNp2, ps1, ps2;  //projectile name file, player sound
	std::string arena, winner, who_headshot, p1texture, p2texture, ulti1, ulti2, ulti_sound1, ulti_sound2;

	//state vars
	bool players_initialized = false;

	bool loginscreen_state = true;
	bool menu_state1 = false;
	bool menu_state2 = false;
	bool arena_selectionState = false;
	bool pregame_state = false;
	bool control_state = false;
	bool playable_state = false;
	bool endgame_state = false;

	bool sound_played = false;
	bool sound_played2 = false;
	bool arenaSelected = false;
	bool choice_given = false;
	bool choice_given2 = false;
	int choicep1 = 0;
	int choicep2 = 0;

	bool checkCollision(Player* player, Projectile* pr, std::string	blast);
	bool checkGhostCollision(Player* player, minnion* ghost);
	void setupFountainProps();
	bool checkCollisionWithFountain(Player* player);

public:
	void update();
	void draw();
	void init();
	Game();
	~Game();
};