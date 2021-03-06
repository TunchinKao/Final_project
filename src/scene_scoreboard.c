#include "scene_scoreboard.h"


extern float VOLUME;
RecArea backbtnArea;
FILE* pfile;
int scorearray[11];
static ALLEGRO_BITMAP* BG;
static ALLEGRO_SAMPLE* BGM;
static ALLEGRO_SAMPLE_ID ID_BGM;
float w, h, rate;
static void init() {
	renew_Record();
	BG = load_bitmap("images/background.png");
	w = al_get_bitmap_width(BG);
	h = al_get_bitmap_height(BG);
	rate = 1.6;
	w = w * rate;
	h = h * rate;
	BG = load_bitmap_resized("images/background.png",w,h);
	BGM = load_audio("Music/Bgm/scoreBGM.ogg");
	ID_BGM = play_bgm(BGM,VOLUME);
}
static void draw() {
	al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgb(0, 0, 0));
	al_draw_bitmap(BG, SCREEN_W /2 - w /2, 0, 0);
	al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(160, 160, 160, 160));

	al_draw_filled_rectangle(backbtnArea.x, backbtnArea.y, backbtnArea.x2, backbtnArea.y2, al_map_rgb(100, 131, 32)); 
	al_draw_text(font_pirulen_24, al_map_rgb(123, 45, 32), backbtnArea.x + 25, backbtnArea.y + 15, 0, "BACK");
	int placey = 180;
	for (int i = 0; i < 10; i++) {
		al_draw_textf(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2 - 200, placey, 0, "%d:    %d ",i + 1, scorearray[i]);
		placey += 40;
	}
}
static void on_mouse_down(int btn, int x, int y, int dz) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, backbtnArea)) {
				game_change_scene(scene_menu_create());
			}
		}

}


static void destroy(void){
	stop_bgm(ID_BGM);
	al_destroy_bitmap(BG);
	al_destroy_sample(BGM);
}
void sorting(int* array,int limit) {
	for (int i = 0; i < limit; i++) {
		for (int j = i; j > 0; j--) {
			if (array[j] > array[j - 1]) {
				int c = array[j];
				array[j] = array[j-1];
				array[j - 1] = c;
			}
			else
				break;
		}
	}
}
void renew_Record() {
	pfile = fopen("Record/score_record.txt", "a+");
	createRecArea(&backbtnArea, 20, 20, 150, 50);
	for (int i = 0; i < 11; i++) {
		fscanf(pfile, "%d", &scorearray[i]);
	}
	fclose(pfile);
	sorting(scorearray, 11);
	pfile = fopen("Record/score_record.txt", "w");
	for (int i = 0; i < 10; i++) {
		game_log("%d", scorearray[i]);
		fprintf(pfile, "%d ", scorearray[i]);
	}
	fclose(pfile);
}
Scene scene_scoreboard_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(scene));
	scene.name = "scoreboard";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_mouse_down = &on_mouse_down;
	// todo: register more event callback functions such as keyboard, mouse, ...
	game_log("start scene created");
	return scene;
}