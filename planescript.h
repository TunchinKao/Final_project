#pragma once

#ifndef PLANESCRIPT_H
#define PLANESCRIPT_H

#include "game.h"
#include "scene_start.h"
#include <allegro5/allegro.h>
#define IMG_SPAN 0.15
typedef struct _movableobject MovableObject;
typedef struct _area Area;
typedef struct _bulletobject bulletObject;
typedef struct _planeobject planeObject;
static const float ENEMY_SHOOTING_COOLDOWN = 0.3f;
static const int MAX_ENEMY_BULLET = 5;
// Return a Scene that has function pointers pointing to the
// functions that will react to the corresponding events.
void dSpeedPlane(planeObject *plane,float ddx, float ddy );
void changePlaneSpeed(planeObject*plane, float vx, float vy);
void changePlanePosition(planeObject* plane);
void changePlaneImg(planeObject* plane);
void changeBulletPostion(bulletObject* bullet);
void smallPlaneScript(planeObject* plane);
void shootingScript(planeObject* plane);
void EnemyBulletcheck(planeObject* plane, MovableObject myself, float *health);
//void EnemyBulletcheck(planeObject* plane);
//bool movableObjectBounderChck(MovableObject* obj);

#endif