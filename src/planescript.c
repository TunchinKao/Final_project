#include "planescript.h"
#include "scene_start.h"

int planeAbility[8][4] = {
	{2,2,2,2},
	{1,3,3,1},
	{3,1,3,1},
	{1,1,3,3},
	{3,1,1,3},
	{1,3,1,3},
	{3,3,1,1},
	{1,2,2,3}
};

extern planeObject Myplane;
extern bulletObject bullets[BULLET_TYPES][MAX_BULLET];
extern planeObject boss_skill_plane[BOSS_SKILL_NUM];
void dSpeedPlane(planeObject* plane, float ddx, float ddy) {
	plane->Data.vx += ddx;
	plane->Data.vy += ddy;
}

void changePlaneSpeed(planeObject* plane, float vx, float vy) {
	plane->Data.vx = vx;
	plane->Data.vy = vy;
}
void changePlanePosition(planeObject* plane) {
	plane->Data.x += plane->Data.vx;
	plane->Data.y += plane->Data.vy;
}

void changeBulletPostion(bulletObject* bullet) {
	bullet->Data.x += bullet->Data.vx;
	bullet->Data.y += bullet->Data.vy;
	if (bullet->Data.y > SCREEN_H + 10)
		bullet->Data.hidden = true;
}
void changePlaneImg(planeObject* plane) {
	if (plane->animationTimer == 0.0f) {
		plane->animationTimer = al_get_time();
	}
	float now = al_get_time();
	if (now - plane->animationTimer < IMG_SPAN) {
		return;
	}
	else {
		plane->sourceX++;
		if (plane->sourceX > 2)
			plane->sourceX = 0;
		plane->animationTimer = now;
	}
}
void resetDirect(planeObject* plane) {
	plane->direct = 0;
}
void smallPlaneScript(planeObject* plane) {
	if (plane->direct == 0)
		plane->direct = 1;
	float speed = 2.0f;
	if (plane->movementTimer <= 1.0f)
		plane->movementTimer = al_get_time();
	double now = al_get_time();
	if (now - plane->movementTimer < 1.5)
		return;
	else {
		//game_log("ChagingDirection tp %d for %p ", plane->direct, plane);
		plane->movementTimer = now;
		switch (plane->direct)
		{
		case 1:
			changePlaneSpeed(plane, speed, speed);
			
			break;
		case 2:
			changePlaneSpeed(plane, -speed, speed);
			break;
		case 3:
			changePlaneSpeed(plane, -speed, -speed);
			break;
		case 4:
			changePlaneSpeed(plane, speed, -speed);
			break;
		default:
			plane->direct = 1;
			break;
		}
		plane->direct++;
		if (plane->direct > 4)
			plane->direct = 1;
	}
}
void secondPlaneScript(planeObject* plane) {
	if (plane->direct == 0)
		plane->direct = 1;/*
	float speed = 2.0f;*/
	if (plane->movementTimer == 0.0f)
		plane->movementTimer = al_get_time();
	double now = al_get_time();
	if (now - plane->movementTimer >= 2.0f)
	{	
		changePlaneSpeed(plane, -plane->Data.vx, plane->Data.vy);
		plane->movementTimer = now;
	}
}
void shootingScript(planeObject* plane) {
	if (plane->Data.hidden || Myplane.Data.hidden)
		return;
	if (plane->shootingTimer == 0.0f) {
		game_log("initialising Timer for %p", plane);
		plane->shootingTimer = al_get_time();
	}
	float now = al_get_time();
	if (now - plane->shootingTimer >= plane->shootingCoolDown) {
		int i;
		for (i = 0; i < MAX_BULLET; i++) {
			if (bullets[plane->shootingBulletType][i].Data.hidden)
				break;
		}
		if (i < MAX_BULLET) {
			plane->shootingTimer = now;
			bullets[plane->shootingBulletType][i].Data.vy = 5;
			bullets[plane->shootingBulletType][i].Data.hidden = false;
			bullets[plane->shootingBulletType][i].Data.x = plane->Data.x;
			bullets[plane->shootingBulletType][i].Data.y = plane->Data.y - plane->Data.h / 2;
			bullets[plane->shootingBulletType][i].whose = 0;
		}
	}
}

void secondShootScript(planeObject* plane) {
	if (plane->Data.hidden || Myplane.Data.hidden)
		return;
	if (plane->shootingTimer == 0.0f) {
		game_log("initialising Timer for %p", plane);
		plane->shootingTimer = al_get_time();
	}
	float now = al_get_time();
	if (now - plane->shootingTimer >= plane->shootingCoolDown) {
		int i;
		for (int k = 0; k < 3; k++) {
			for (i = 0; i < MAX_BULLET; i++) {
				if (bullets[plane->shootingBulletType][i].Data.hidden)
					break;
			}
			if (i < MAX_BULLET) {
				plane->shootingTimer = now;
				bullets[plane->shootingBulletType][i].Data.vy = 3;
				bullets[plane->shootingBulletType][i].Data.vx = 1.5 - k;
				bullets[plane->shootingBulletType][i].Data.hidden = false;
				bullets[plane->shootingBulletType][i].Data.x = plane->Data.x + plane->Data.w / 2;
				bullets[plane->shootingBulletType][i].Data.y = plane->Data.y + bullets[plane->shootingBulletType][i].Data.h / 2;
				bullets[plane->shootingBulletType][i].whose = 0;
				
			}
		 }
	}
	
}
void BossmovementScript(planeObject* plane) {
	static bool flag = true;
	if (plane->Data.y < 25) {
		plane->Data.vy = 2;
	}
	
	else {
		if (plane->movementTimer == 0.0f) {
			plane->movementTimer = al_get_time();
		}
		float now = al_get_time();
		
		if (now - plane->movementTimer > 1.5f) {
			if (flag) {
				plane->Data.vy = (float)rand() / RAND_MAX * 6;
				flag = !flag;
			}
			else {
				plane->Data.vy = -plane->Data.vy;
				flag = !flag;
			}
			plane->movementTimer = now;
		}
		if (plane->Data.x <= 150 || plane->Data.x >= SCREEN_W - 150) {
			plane->Data.vx = -plane->Data.vx;
		}
	
	}
}
void BossshootingScript(planeObject* plane) {
	if (plane->Data.hidden || Myplane.Data.hidden)
		return;
	if (plane->shootingTimer == 0.0f) {
		game_log("initialising Timer for %p", plane);
		plane->shootingTimer = al_get_time();
	}
	float now = al_get_time();
	if (now - plane->shootingTimer >= plane->shootingCoolDown) {
		int i;
		for (int k = 0; k < 5; k++) {
			for (i = 0; i < MAX_BULLET; i++) {
				if (bullets[plane->shootingBulletType][i].Data.hidden)
					break;
			}
			if (i < MAX_BULLET) {
				plane->shootingTimer = now;
				bullets[plane->shootingBulletType][i].Data.vx = (2.5 - k)*1.5;
				bullets[plane->shootingBulletType][i].Data.vy = 5;
				bullets[plane->shootingBulletType][i].Data.hidden = false;
				bullets[plane->shootingBulletType][i].Data.x = plane->Data.x;
				bullets[plane->shootingBulletType][i].Data.y = plane->Data.y + plane->Data.h / 3;
				bullets[plane->shootingBulletType][i].whose = 0;
			}

		}
	}
}
//
//void BossSkillScript(planeObject * plane) {
//	if (plane->SkillTimer == 0.0f) {
//		plane->SkillTimer = al_get_time();
//	}
//	float now = al_get_time();
//	if ((now - plane->SkillTimer > (15.0f - (float)rand() / RAND_MAX * 5 ))&& (boss_skill_plane[0].Data.x < -50)){
//		game_log("%f %f", boss_skill_plane[0].Data.x, boss_skill_plane[0].Data.y);
//		for (int i = 0; i < 6; i++) {
//			float y = SCREEN_H / 2;
//			boss_skill_plane[i].Data.y = y + (float)rand() / RAND_MAX * SCREEN_H / 2;
//			if (i % 2 == 0){
//				boss_skill_plane[i].Data.x = SCREEN_W + 50;
//				boss_skill_plane[i].sourceY = 1;
//				boss_skill_plane[i].Data.vx = 2;
//			}
//			else{
//				boss_skill_plane[i].Data.x = - 50;
//				boss_skill_plane[i].sourceY = 2;
//				boss_skill_plane[i].Data.vx = -2;
//
//			}
//		}
//
//		plane->SkillTimer = now;
//	}
//	
//}
//void circle_shooting_Script(planeObject* plane, int bullet_type) {
//
//}
