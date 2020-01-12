#include "planescript.h"
#include "scene_start.h"


extern MovableObject Myplane;
extern bulletObject bullets[BULLET_TYPES][MAX_BULLET];
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
		plane->animationCounter++;
		plane->sourceX++;
		if (plane->animationCounter > 2)
			plane->animationCounter = 0;
		if (plane->sourceX > 2)
			plane->sourceX = 0;
		plane->animationTimer = now;
	}
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

void shootingScript(planeObject* plane) {
	if (plane->Data.hidden || Myplane.hidden)
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
			bullets[plane->shootingBulletType][i].Data.y = plane->Data.y + bullets[plane->shootingBulletType][i].Data.h / 2;
			bullets[plane->shootingBulletType][i].whose = 0;
		}
	}
}
//void circle_shooting_Script(planeObject* plane, int bullet_type) {
//
//}
