// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_START_H
#define SCENE_START_H

#include "game.h"
#include "planescript.h"
#include "object.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include "scene_menu.h"
#include "scene_scoreboard.h"
#include "game.h"
#include "utility.h"
#include "shared.h"
#include <math.h>
#define MAX_ENEMY 5
#define MAX_BULLET 300
#define ENEMY_TYPES 2
#define BULLET_TYPES 4
#define BOSS_HEALTH 500
#define BOSS_SKILL_NUM 6
static int SCORE = 0;
struct _movableobject;
struct _area;
struct _bulletobject;
struct _planeobject;
typedef struct _movableobject MovableObject;
typedef struct _area Area;
typedef struct _bulletobject bulletObject;
typedef struct _planeobject planeObject;
struct _movableobject {
	float x, y;
	int w, h;
	int collisionw, collisionh;
	float vx, vy;
	bool hidden;
	ALLEGRO_BITMAP* img;
};

struct _area {
	float top, bottom, left, right;
};

struct _bulletobject {
	MovableObject Data;
	bool whose; // 0 enemy // 1 mine
	float damage;
};

struct _planeobject {
	MovableObject Data;
	float health;
	float movementTimer;
	float animationTimer;
	float SkillTimer;
	int direct;
	int sourceX;
	int sourceY;
	float shootingTimer;
	float shootingCoolDown;
	int shootingBulletType;
	int prize;

};


static void init(void);
static void update(void);
static void draw_movable_object(MovableObject obj);
static void draw(void);
static void destroy(void);

static void update_BG_area(RecArea* bg);

// self function
bool MovableObjectCollision(MovableObject target1, MovableObject target2);
void buildplane(planeObject* plane,
	ALLEGRO_BITMAP* img,
	float health,
	float vx,
	float vy,
	int dcollisionw,
	int dcollisionh,
	float shootingCoolDown,
	int shootingBulletType,
	int prize);
void buildbullet(bulletObject* bullet,
	ALLEGRO_BITMAP* img,
	float damage,
	float vx,
	float vy,
	int dcollisionw,
	int dcollisionh);
void drawplane(planeObject* plane);
//static void draw_plane_object(planeObject obj);
void selfBounderCheck();
void put_plane(planeObject* plane, int x, int y);
void movOjBounederCheck(MovableObject* MoBj);
void drawSelfHealth();
void drawSCORE();
// update funcs
void all_bullet_planes_collision_check();
//script


//void dSpeedPlane(planeObject* plane, float ddx, float ddy);
//void changePlaneSpeed(planeObject* plane, float vx, float vy);
planeObject Myplane;
planeObject BOSS;
static planeObject enemy_plane[ENEMY_TYPES][MAX_ENEMY]; // normal
planeObject boss_skill_plane[BOSS_SKILL_NUM];
bulletObject bullets[BULLET_TYPES][MAX_BULLET]; // 0 normal

static int SCORE;

static const float MAX_COOLDOWN = 0.1f;
static double last_shoot_timestamp;
static ALLEGRO_SAMPLE* bgm;
static ALLEGRO_SAMPLE_ID bgm_id;
static bool draw_gizmos;


Scene scene_start_create(void);
#endif
