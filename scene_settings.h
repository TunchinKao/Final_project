// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H
#include <stdlib.h>
#include "game.h"
#include "scene_start.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_color.h"
#include "allegro5/allegro_font.h"
#include "scene_menu.h"
RecArea backArea;
RecArea volLargeBtnArea;
RecArea volSmallBtnArea;
RecArea nextImgBtnArea;
RecArea preImgBtnArea;
RecArea characterDemoArea;
// Return a Scene that has function pointers pointing to the
// functions that will react to the corresponding events.
static void init();
static void on_mouse_down(int btn, int x, int y, int dz);
static void destroy();

Scene scene_settings_create(void);
#endif