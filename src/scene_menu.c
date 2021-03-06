#include "scene_menu.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"
#include "utility.h"
#include "shared.h"
#include "scene_start.h"
#include "scene_settings.h"
#include "scene_scoreboard.h"

extern float VOLUME;
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
// title is here
static const char* txt_title = "DRAGON ADVENTURE";
static const char* txt_info = "Press enter key \"enter\" to start";
static const char* txt_info2 = "\"Esc\" to close";
static ALLEGRO_BITMAP* img_background;
// [HACKATHON 3-1]
// TODO: Declare 2 variables for storing settings images.
// Uncomment and fill in the code below.
static ALLEGRO_BITMAP* img_settings;
static ALLEGRO_BITMAP* img_settings2;
static ALLEGRO_SAMPLE* bgm; 
static ALLEGRO_SAMPLE_ID bgm_id;

static void init(void);
static void draw(void);
static void destroy(void);
static void on_key_down(int keycode);
RecArea settingButtonArea;
RecArea startButtonArea;
RecArea scoreboardButtonArea;
static void init(void) {
	img_background = load_bitmap_resized("images/menu_background.png", SCREEN_W, SCREEN_H);
	createRecArea(&settingButtonArea, SCREEN_W - 48, 10, 38, 38);
	createRecArea(&startButtonArea, 400, SCREEN_H / 2, 400, 75);
	createRecArea(&scoreboardButtonArea, 400, SCREEN_H / 2 + 100, 400, 75);

	// [HACKATHON 3-4]
	// TODO: Load settings images.
	// Uncomment and fill in the code below.
	img_settings = al_load_bitmap("UIImage/settings.png");
	img_settings2 = al_load_bitmap("UIImage/settings2.png");
	// Can be moved to shared_init to decrease loading time.
	bgm = load_audio("Music/Bgm/Dragon_and_Toast_ok.ogg");
	//game_log("VOLUME: %f!", VOLUME);
	bgm_id = play_bgm(bgm, VOLUME);
	game_log("Menu scene initialized");
}

static void draw(void) {
	al_draw_bitmap(img_background, 0, 0, 0);
	
	al_draw_filled_rectangle(startButtonArea.x,
							startButtonArea.y,
							startButtonArea.x2,
							startButtonArea.y2,
							al_map_rgb(0, 204, 0));
	al_draw_filled_rectangle(scoreboardButtonArea.x,
							scoreboardButtonArea.y,
							scoreboardButtonArea.x2,
							scoreboardButtonArea.y2,
							al_map_rgb(204, 204, 0));
	if (pnt_in_rect(mouse_x, mouse_y, settingButtonArea))
		al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
	else
		al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
	al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, txt_title);
	/*al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 100, 0, txt_info);
	al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, txt_info2);*/
	al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), startButtonArea.x + 50, startButtonArea.y + 20, 0, "START");
	al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), scoreboardButtonArea.x + 60, scoreboardButtonArea.y + 20, 0, "ScoreBoard");
	al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 10, SCREEN_H -  30, 0, txt_info2);

}

static void on_key_down(int keycode) {
	if (keycode == ALLEGRO_KEY_ENTER)
		game_change_scene(scene_start_create());
}

static void on_mouse_down(int btn, int x, int y, int dz) {
	if (btn == 1) {
		if (pnt_in_rect(x, y,  settingButtonArea))
			game_change_scene(scene_settings_create());
		else if (pnt_in_rect(x, y, startButtonArea))
			game_change_scene(scene_start_create());
		else if (pnt_in_rect(x, y, scoreboardButtonArea))
			game_change_scene(scene_scoreboard_create());
	}
}

static void destroy(void) {
	al_destroy_sample(bgm);
	al_destroy_bitmap(img_background);
	// [HACKATHON 3-6]
	// TODO: Destroy the 2 settings images.
	// Uncomment and fill in the code below.
	al_destroy_bitmap(img_settings);
	al_destroy_bitmap(img_settings2);
	stop_bgm(bgm_id);
	game_log("Menu scene destroyed");              
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_menu_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Menu";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	// [HACKATHON 3-8]
	// TODO: Register on_mouse_down.
	// Uncomment the code below.
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as update, mouse, ...
	game_log("Menu scene created");
	return scene;
}
