
#include "scene_start.h"
extern bool draw_gizmos;
bool MovableObjectCollision(MovableObject target1, MovableObject target2) {
	if (target1.hidden || target2.hidden)
		return false;
	Area Area1;
	Area1.left = target1.x - target1.collisionw / 2;
	Area1.right = target1.x + target1.collisionw / 2;
	Area1.top = target1.y - target1.collisionh / 2;
	Area1.bottom = target1.y + target1.collisionh / 2;
	Area Area2;
	Area2.left = target2.x - target2.collisionw / 2;
	Area2.right = target2.x + target2.collisionw / 2;
	Area2.top = target2.y - target2.collisionh / 2;
	Area2.bottom = target2.y + target2.collisionh / 2;

	if (Area1.bottom < Area2.top ||
		Area1.top > Area2.bottom ||
		Area1.right < Area2.left ||
		Area1.left > Area2.right)
	{
		// no collision
		return false;
	}
	return true;
}
void buildplane(planeObject* plane,
	ALLEGRO_BITMAP* img,
	float health,
	float vx,
	float vy,
	int dcollisionw,
	int dcollisionh,
	float shootingCoolDown,
	int shootingBulletType,
	int prize) {
	plane->Data.img = img;
	plane->Data.w = al_get_bitmap_width(plane->Data.img) / 3;
	plane->Data.h = al_get_bitmap_height(plane->Data.img) / 4;
	plane->Data.collisionw = plane->Data.w - dcollisionw * 2;
	plane->Data.collisionh = plane->Data.h - dcollisionh * 2;
	plane->Data.x = plane->Data.w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - plane->Data.w);
	//plane->Data.x = -250;
	plane->Data.y = 20;
	plane->Data.vx = vx;
	plane->Data.vy = vy;
	plane->Data.hidden = false;
	plane->health = health;
	plane->movementTimer = 0.0f;
	plane->animationTimer = 0.0f;
	plane->SkillTimer = 0.0f;
	plane->direct = 0;
	plane->sourceX = 0;
	plane->sourceY = 0;
	plane->shootingTimer = 0.0f;
	plane->shootingCoolDown = shootingCoolDown;
	plane->shootingBulletType = shootingBulletType;
	plane->prize = prize;
	//game_log("BUILD plane successful %p", plane);
}
void buildbullet(bulletObject* bullet,
	ALLEGRO_BITMAP* img,
	float damage,
	float vx,
	float vy,
	int dcollisionw,
	int dcollisionh) {
	bullet->Data.img = img;
	bullet->Data.w = al_get_bitmap_width(img);
	bullet->Data.h = al_get_bitmap_height(img);
	bullet->Data.collisionw = bullet->Data.w - dcollisionw * 2;
	bullet->Data.collisionh = bullet->Data.h - dcollisionh * 2;
	bullet->Data.vx = vx;
	bullet->Data.vy = vy;
	bullet->Data.hidden = true;
	bullet->damage = damage;
	bullet->whose = -1;
}
void put_plane(planeObject* plane, int x, int y) {
	plane->Data.x = plane->Data.w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - plane->Data.w);
	plane->Data.y = y;
	plane->Data.hidden = false;
}
void movOjBounederCheck(MovableObject* MoBj) {
	if (MoBj->x < 0 - 100 || MoBj->x > SCREEN_W + 100 || MoBj->y < 0 - 100 || MoBj->y > SCREEN_H + 100)
		MoBj->hidden = true;
}