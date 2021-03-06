#pragma once
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_SCOREBOARD_H
#define SCENE_SCOREBOARD_H

#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"
#include <math.h>

void sorting(int* array,int limit);
void renew_Record();
Scene scene_scoreboard_create(void);
#endif
