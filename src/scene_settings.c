// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include "scene_start.h"
#include "allegro5/timer.h"
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */
#define CHARARCTER_NUM 8

extern float VOLUME;
extern int MydraIndex;
extern char characterlist[8][2][40];	

const double img_changing_span = 0.15;
int sourceX;
int sourceY;
int width;
int height;
int nowerId;
// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
ALLEGRO_BITMAP* LargeBtn;
ALLEGRO_BITMAP* SmallBtn;
ALLEGRO_BITMAP* nextCharacBtn;
ALLEGRO_BITMAP* preCharacBtn;
ALLEGRO_BITMAP* tmp;
ALLEGRO_FONT* font_pirulen_26;
static float timer;
static void init(void) {
	sourceX = 0;
	sourceY = 0;
	timer = al_get_time();
	font_pirulen_26 = load_font("pirulen.ttf", 32); 
	nowerId = MydraIndex =  0;
	createRecArea(&volLargeBtnArea, 600, 270, 100, 100);
	createRecArea(&volSmallBtnArea, 400, 270, 100, 100);
	createRecArea(&nextImgBtnArea, 600, 400, 100, 100);
	createRecArea(&preImgBtnArea, 300, 400, 100, 100);
	LargeBtn = load_bitmap_resized("UIImage/vollarger.png",volLargeBtnArea.w,volLargeBtnArea.h);
	SmallBtn = load_bitmap_resized("UIImage/volsmaller.png", volSmallBtnArea.w, volSmallBtnArea.h);
	nextCharacBtn = load_bitmap_resized("UIImage/fast-forward-button.png",nextImgBtnArea.w, nextImgBtnArea.h);
	preCharacBtn = load_bitmap_resized("UIImage/fast-backward-button.png", nextImgBtnArea.w, nextImgBtnArea.h);
	createRecArea(&backArea,150, 200, 150, 50);
	createRecArea(&characterDemoArea, 465, 425, 100, 100);
	tmp = al_load_bitmap(characterlist[MydraIndex][0]);
	width = al_get_bitmap_width(tmp) /3;
	height = al_get_bitmap_height(tmp) / 4;
}
static void update(void) {
	if (nowerId != MydraIndex) {
		tmp = al_load_bitmap(characterlist[MydraIndex][0]);
		nowerId = MydraIndex;
	}
	double now = al_get_time();
	if (now - timer > img_changing_span) {
		sourceX++;
		if (sourceX > 2)
			sourceX = 0;
		timer = now;
	}
}
static void draw(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(backArea.x, backArea.y,backArea.x2, backArea.y2, al_map_rgb(255, 128, 0));
	al_draw_text(font_pirulen_26, al_map_rgb(160, 160, 160), 155, 205, 0, "BACK");
	al_draw_text(font_pirulen_26, al_map_rgb(160, 160, 160), 155, 275, 0, "VOLUME");
	al_draw_textf(font_pirulen_26, al_map_rgb(160, 160, 160), 155, 350, 0, "%.1f", VOLUME);

	al_draw_filled_rectangle(volSmallBtnArea.x, volSmallBtnArea.y, volSmallBtnArea.x2, volSmallBtnArea.y2, al_map_rgb(153,153,255));
	al_draw_filled_rectangle(volLargeBtnArea.x, volLargeBtnArea.y, volLargeBtnArea.x2, volLargeBtnArea.y2, al_map_rgb(153, 153, 255));
	al_draw_bitmap(LargeBtn, volLargeBtnArea.x, volLargeBtnArea.y, 0);
	al_draw_bitmap(SmallBtn, volSmallBtnArea.x, volSmallBtnArea.y, 0);
	al_draw_bitmap(nextCharacBtn, nextImgBtnArea.x, nextImgBtnArea.y, 0);
	al_draw_bitmap(preCharacBtn, preImgBtnArea.x, preImgBtnArea.y, 0);
	
	al_draw_bitmap_region(tmp, sourceX * width, sourceY * height, width, height, characterDemoArea.x, characterDemoArea.y, 0);
	
}
static void on_mouse_down(int btn, int x, int y, int dz) {
	if (btn == 1) {
		
		if (pnt_in_rect(x, y, backArea))
			game_change_scene(scene_menu_create());
		else if (pnt_in_rect(x, y, volLargeBtnArea)) {
			VOLUME +=0.2;
		}
		else if (pnt_in_rect(x, y, volSmallBtnArea)) {
			VOLUME -= 0.2;
		}
		else if (pnt_in_rect(x, y, nextImgBtnArea)) {
			MydraIndex++;
			if (MydraIndex >= CHARARCTER_NUM)
				MydraIndex = 0;
		}else if (pnt_in_rect(x, y, preImgBtnArea)) {
			MydraIndex++;
			if (MydraIndex < 0)
				MydraIndex = CHARARCTER_NUM -1;
		}

	}
}
static void destroy(void) {
	al_destroy_font(font_pirulen_26);
	al_destroy_bitmap(tmp);
}
// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "setting";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_mouse_down = &on_mouse_down;
	game_log("Settings scene created");
	return scene;
}

