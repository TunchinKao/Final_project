#include "scene_start.h"

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
extern float VOLUME;
static ALLEGRO_BITMAP* img_background;
static ALLEGRO_BITMAP* img_plane;
static ALLEGRO_BITMAP* img_enemy;
static ALLEGRO_BITMAP* img_bullet;
static ALLEGRO_BITMAP* img_enemy_bullet;
static ALLEGRO_BITMAP* pause_button;
static ALLEGRO_BITMAP* continue_button;
static ALLEGRO_BITMAP* pause_button2;
static ALLEGRO_BITMAP* continue_button2;
static ALLEGRO_BITMAP* pausing_cover;
static ALLEGRO_BITMAP* originBitmap;
const int bgConst = 150, bgConst2 = 200; // move to SCREEN_H - this // when y <= this
RecArea buttonArea;
RecArea btMenuArea;
RecArea btRestartArea;
RecArea backbgArea;
static bool is_pausing = false;
static bool is_failed = false;
static bool is_winned = false;
const float buttonrate = 0.1;
static float buttonWidth; // Original
static float buttonHeight; // Original
static float backImgHeight;
static float backImgWidth;
static float bg_x_velocity = 0;
static float bg_y_velocity = -1.5;

static void init(void) {
	int i;
	originBitmap = load_bitmap("MapImage/linktothepast.jpg");
	backImgHeight = al_get_bitmap_height(originBitmap);
	backImgWidth = al_get_bitmap_width(originBitmap);
	img_background = load_bitmap_resized("MapImage/linktothepast.jpg", backImgWidth * 2, backImgHeight * 2);
	backImgHeight = al_get_bitmap_height(img_background);
	backImgWidth = al_get_bitmap_width(img_background);
	//game_log("img_backgroun %d %d", backImgWidth, backImgHeight);

	createRecArea(&backbgArea, 500, backImgHeight - SCREEN_H, 500 + SCREEN_W, backImgHeight);
	createRecArea(&btMenuArea, SCREEN_W / 2 - 150, SCREEN_H / 2 - 25, 300, 50);
	createRecArea(&btRestartArea, SCREEN_W / 2 - 150, SCREEN_H / 2 + 75, 300, 50);

	pause_button = load_bitmap("GUI/pause-button.png");
	continue_button = load_bitmap("GUI/play-button.png");
	pause_button2 = load_bitmap("GUI/pause-button2.png");
	continue_button2 = load_bitmap("GUI/play-button2.png");
	pausing_cover = al_create_bitmap(SCREEN_W, SCREEN_H);
	buttonWidth = al_get_bitmap_width(pause_button);
	buttonHeight = al_get_bitmap_height(pause_button);
	createRecArea(&buttonArea, 20, 20, al_get_bitmap_width(pause_button) * buttonrate, al_get_bitmap_height(pause_button) * buttonrate);


	img_plane = Myplane.img = load_bitmap("Character/plane.png");
	Myplane.x = 400;
	Myplane.y = 500;
	Myplane.w = al_get_bitmap_width(Myplane.img);
	Myplane.h = al_get_bitmap_height(Myplane.img);
	Myplane.hidden = false;
	My_Bullet_Type = 0;
	MyHealth = 100;
	SCORE = 0;

	img_enemy_bullet = al_load_bitmap("image12.png");
	for (int j = 0; j < ENEMY_TYPES; j++) {
		img_enemy = load_bitmap("Character/insect/insector04.png");
		for (i = 0; i < MAX_ENEMY; i++) {
			buildplane(&enemy_plane[j][i], img_enemy, 0, 3, 1);
		}
	}
	// Initialize bullets.
	for (int j = 0; j < BULLET_TYPES; j++) {
		img_bullet = al_load_bitmap("image12.png");
		for (int i = 0; i < MAX_BULLET; i++) {
			bullets[j][i].Data.img = img_bullet;
			bullets[j][i].Data.w = al_get_bitmap_width(img_bullet);
			bullets[j][i].Data.h = al_get_bitmap_height(img_bullet);
			bullets[j][i].Data.vx = 0;
			bullets[j][i].Data.vy = -5;
			bullets[j][i].Data.hidden = true;
			bullets[j][i].damage = 10.0f;
		}
	}
	// Can be moved to shared_init to decrease loading time.
	bgm = load_audio("mythica.ogg");
	game_log("Start scene initialized");
	bgm_id = play_bgm(bgm, VOLUME);
	//game_log("Plane: %d %d", Myplane.w, Myplane.y);
}

static void update(void) {
	if (is_pausing)
		return;
	update_BG_area(&backbgArea);
	// My location Update
	Myplane.vx = Myplane.vy = 0;
	if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
		Myplane.vy -= 1;
	if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
		Myplane.vy += 1;
	if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A]) {
		Myplane.vx -= 1;
		//bg_x_velocity -= 0.5;
	}
	if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D]) {
		Myplane.vx += 1;
		//bg_x_velocity += 0.5;
	}

	Myplane.y += Myplane.vy * 4 * (Myplane.vx ? 0.71f : 1); // 0.71 is (1/sqrt(2)).
	Myplane.x += Myplane.vx * 4 * (Myplane.vy ? 0.71f : 1);
	//game_log(" Position%f %f",Myplane.x, Myplane.y);
	selfBounderCheck();
	// Update bullets coordinates.
	for (int j = 0; j < BULLET_TYPES; j++) {
		for (int i = 0; i < MAX_BULLET; i++) {
			movOjBounederCheck(&(bullets[j][i].Data));
			if (bullets[j][i].Data.hidden)
				continue;
			bullets[j][i].Data.x += bullets[j][i].Data.vx;
			bullets[j][i].Data.y += bullets[j][i].Data.vy;
			movOjBounederCheck(&(bullets[j][i].Data));
		}
	}
	// update enemy
	is_winned = true;
	for (int j = 0; j < ENEMY_TYPES; j++)
		for (int i = 0; i < MAX_ENEMY; i++) {
			if (enemy_plane[j][i].Data.hidden)
				continue;
			if (enemy_plane[j][i].health <= 0) {
				enemy_plane[j][i].Data.hidden = true;
				SCORE += enemy_plane[j][i].prize;
				continue;
			}
			
			is_winned = false;
			changePlaneImg(&enemy_plane[j][i]);
			smallPlaneScript(&enemy_plane[j][i]);
			changePlanePosition(&enemy_plane[j][i]);
			shootingScript(&enemy_plane[j][i]);
		}
	all_bullet_planes_collision_check();
	double now = al_get_time();
	int i;
	if ((mouse_state[1]) && now - last_shoot_timestamp >= MAX_COOLDOWN) {
		for (i = 0; i < MAX_BULLET; i++) {
			if (bullets[My_Bullet_Type][i].Data.hidden)
				break;
		}
		if (i < MAX_BULLET) {
			last_shoot_timestamp = now;
			bullets[My_Bullet_Type][i].Data.vy = -5;
			bullets[My_Bullet_Type][i].Data.hidden = false;
			bullets[My_Bullet_Type][i].Data.x = Myplane.x;
			bullets[My_Bullet_Type][i].Data.y = Myplane.y + bullets[My_Bullet_Type][i].Data.h / 2;
			bullets[My_Bullet_Type][i].whose = 1;
		}
	}
	if (MyHealth <= 0) {
		Myplane.hidden = true;
		is_failed = true;
	}

}

static void draw_movable_object(MovableObject obj) {
	if (obj.hidden)
		return;
	al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
	if (draw_gizmos) {
		al_draw_rectangle(round(obj.x - obj.w / 2), round(obj.y - obj.h / 2),
			round(obj.x + obj.w / 2) + 1, round(obj.y + obj.h / 2) + 1, al_map_rgb(255, 0, 0), 0);
	}
}

static void draw(void) {
	int i, j;
	ALLEGRO_BITMAP* subBitmap = al_create_sub_bitmap(img_background, backbgArea.x, backbgArea.y, backbgArea.w, backbgArea.h);
	al_draw_bitmap(subBitmap, 0, 0, 0);
	al_destroy_bitmap(subBitmap);
	// Draw all bullets in your bullet array.
	for (j = 0; j < BULLET_TYPES; j++)
		for (i = 0; i < MAX_BULLET; i++)
			draw_movable_object(bullets[j][i].Data);
	draw_movable_object(Myplane);
	if (is_failed) {
		//al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(64, 64, 64, 200));
		al_draw_text(font_pirulen_32, al_map_rgb(255, 0, 0), SCREEN_W / 2 - 100, SCREEN_H / 2 - 150, 0, "DEFEAT");
	}else if(is_winned)
		al_draw_text(font_pirulen_32, al_map_rgb(0, 0, 255), SCREEN_W / 2 - 100, SCREEN_H / 2 - 150, 0, "VICTORY");

	for (j = 0; j < ENEMY_TYPES; j++) {
		for (i = 0; i < MAX_ENEMY; i++) {
			drawplane(&enemy_plane[j][i]);
		}
	}

	drawSelfHealth();
	drawSCORE();
	if (is_pausing || is_failed || is_winned) {
		al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(64, 64, 64, 200));
		al_draw_filled_rectangle(btMenuArea.x, btMenuArea.y, btMenuArea.x2, btMenuArea.y2, al_map_rgb(51, 51, 255));
		al_draw_text(font_pirulen_24, al_map_rgb(0, 25, 51), btMenuArea.x + 25, btMenuArea.y + 5, 0, "back to menu");
		al_draw_filled_rectangle(btRestartArea.x, btRestartArea.y, btRestartArea.x2, btRestartArea.y2, al_map_rgb(54, 23, 167));
		al_draw_text(font_pirulen_24, al_map_rgb(132, 54, 34), btRestartArea.x + 25, btRestartArea.y + 5, 0, "Restart");
	}

		if (pnt_in_rect(mouse_x, mouse_y, buttonArea)) {
			if (is_pausing)
				al_draw_scaled_bitmap(continue_button, 0, 0, buttonWidth, buttonHeight, buttonArea.x, buttonArea.y, buttonArea.w, buttonArea.h, 0);
			else
				if(!is_failed && !is_winned)
				al_draw_scaled_bitmap(pause_button, 0, 0, buttonWidth, buttonHeight, buttonArea.x, buttonArea.y, buttonArea.w, buttonArea.h, 0);
		}
		else {
			if (is_pausing)
				al_draw_scaled_bitmap(continue_button2, 0, 0, buttonWidth, buttonHeight, buttonArea.x, buttonArea.y, buttonArea.w, buttonArea.h, 0);
			else
				if(!is_failed && !is_winned)
				al_draw_scaled_bitmap(pause_button2, 0, 0, buttonWidth, buttonHeight, buttonArea.x, buttonArea.y, buttonArea.w, buttonArea.h, 0);
		}
	

}

static void destroy(void) {
	al_destroy_bitmap(img_background);
	al_destroy_bitmap(img_plane);
	al_destroy_bitmap(img_enemy);
	al_destroy_bitmap(img_bullet);
	al_destroy_bitmap(img_enemy_bullet);
	al_destroy_bitmap(pause_button);
	al_destroy_bitmap(continue_button);
	al_destroy_bitmap(pause_button2);
	al_destroy_bitmap(continue_button2);
	al_destroy_bitmap(pausing_cover);
	al_destroy_sample(bgm);

	//  Destroy your bullet image.
	stop_bgm(bgm_id);
	game_log("Start scene destroyed");
}
static void update_BG_area(RecArea* bg) {
	if (bg->x >= 0 && bg->x + SCREEN_W <= backImgWidth) {
		bg->x += bg_x_velocity;
		bg->x2 += bg_x_velocity;
	}
	bg->y += bg_y_velocity;
	bg->y2 += bg_y_velocity;
	bg->w = bg->x2 - bg->x;
	if (bg->y <= bgConst2) {
		bg->y = backImgHeight - SCREEN_H - bgConst;
		bg->y2 = backImgHeight - SCREEN_H - bgConst;
	}
}
static void on_key_down(int keycode) {
	if (keycode == ALLEGRO_KEY_TAB)
		draw_gizmos = !draw_gizmos;
}
static void on_mouse_down(int btn, int x, int y, int dz) {
	if (btn == 1) {
		if (pnt_in_rect(x, y, buttonArea)) {
			//game_log("mouse click the button!");
			if (!is_pausing)
				is_pausing = true;
			else
				is_pausing = false;
		}
		if (is_pausing || is_failed || is_winned) {
			if (pnt_in_rect(x, y, btMenuArea)) {
				is_pausing = false;
				is_failed = false;
				game_change_scene(scene_menu_create());
			}
			else if (pnt_in_rect(x, y, btRestartArea)) {
				is_pausing = false;
				is_failed = false;
				game_change_scene(scene_start_create());
			}
		}
	}
}
// TODO: Add more event callback functions such as keyboard, mouse, ...
bool MovableObjectCollision(MovableObject target1, MovableObject target2) {
	if (target1.hidden || target2.hidden)
		return false;
	Area Area1;
	Area1.left = target1.x - target1.w / 2;
	Area1.right = target1.x + target1.w / 2;
	Area1.top = target1.y - target1.h / 2;
	Area1.bottom = target1.y + target1.h / 2;
	Area Area2;
	Area2.left = target2.x - target2.w / 2;
	Area2.right = target2.x + target2.w / 2;
	Area2.top = target2.y - target2.h / 2;
	Area2.bottom = target2.y + target2.h / 2;

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
	float bspeedx,
	float bspeedy,
	float shootcool) {
	plane->Data.img = img;
	plane->Data.w = al_get_bitmap_width(plane->Data.img) / 3;
	plane->Data.h = al_get_bitmap_height(plane->Data.img) / 4;
	plane->Data.x = plane->Data.w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - plane->Data.w);
	plane->Data.y = 80;
	plane->Data.hidden = false;
	plane->health = 100.0f;
	plane->movementTimer = 0.0f;
	plane->animationTimer = 0.0f;
	plane->animationCounter = 0;
	plane->direct = 0;
	plane->sourceX = 0;
	plane->sourceY = 0;
	plane->shootingTimer = 0.0f;
	plane->shootingCoolDown = shootcool;
	plane->shootingBulletType = 0;
	plane->prize = 10;
	//game_log("BUILD plane successful %p", plane);
}

void drawplane(planeObject* plane) {
	if (plane->Data.hidden)
		return;
	al_draw_bitmap_region(plane->Data.img,
		plane->sourceX * plane->Data.w,
		plane->sourceY * plane->Data.h,
		plane->Data.w,
		plane->Data.h,
		plane->Data.x,
		plane->Data.y,
		0);
}
void selfBounderCheck() {
	// Limit the plane's collision box inside the frame.
	if (Myplane.x < 0 + Myplane.h / 2)
		Myplane.x = Myplane.w / 2;
	else if (Myplane.x > (SCREEN_W - Myplane.w / 2))
		Myplane.x = SCREEN_W - Myplane.w / 2;
	if (Myplane.y < 0 + Myplane.h / 2)
		Myplane.y = Myplane.h / 2;
	else if (Myplane.y > (SCREEN_H - Myplane.h / 2))
		Myplane.y = SCREEN_H - Myplane.h / 2;
}

void movOjBounederCheck(MovableObject* MoBj) {
	if (MoBj->x < 0 - 100 || MoBj->x > SCREEN_W + 100 || MoBj->y < 0 - 100 || MoBj->y > SCREEN_H + 100)
		MoBj->hidden = true;
}
void drawSelfHealth() {
	if (MyHealth > 0)
		al_draw_filled_rectangle(10, SCREEN_H - 40, 10 + (200 / 100) * MyHealth, SCREEN_H - 10, al_map_rgb(51, 255, 51));
}
void drawSCORE() {
	al_draw_textf(font_pirulen_32, al_map_rgb(127, 0, 255), SCREEN_W - 50, 10, 0, "%d", SCORE);
}
void all_bullet_planes_collision_check() {
	for (int j = 0; j < BULLET_TYPES; j++) {
		for (int i = 0; i < MAX_BULLET; i++) {
			if (bullets[j][i].Data.hidden)
				continue;
			if (bullets[j][i].whose) {
				for (int k = 0; k < ENEMY_TYPES; k++) {
					for (int q = 0; q < MAX_ENEMY; q++) {
						if (enemy_plane[k][q].Data.hidden)
							continue;
						if (MovableObjectCollision(bullets[j][i].Data, enemy_plane[k][q].Data)) {
							enemy_plane[k][q].health -= bullets[j][i].damage;
							bullets[j][i].Data.hidden = true;
						}
					}
				}
			}
			else {
				if (MovableObjectCollision(bullets[j][i].Data, Myplane)) {
					MyHealth -= bullets[j][i].damage;
					bullets[j][i].Data.hidden = true;
				}
			}
		}
	}
}

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_start_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}