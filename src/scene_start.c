#include "scene_start.h"
#include <math.h>
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
#define STAGE_NUM 3
extern float VOLUME;
extern int MydraIndex;
extern char characterlist[8][2][40];
extern char enemylist[4][40];
extern char bulletImglist[4][40];
extern int planeAbility[8][4];

int stageflag;
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
static ALLEGRO_BITMAP* BOSS_img;
static ALLEGRO_SAMPLE* fireball;
static ALLEGRO_SAMPLE* fightingBGM;
static ALLEGRO_SAMPLE* BossBGM;
static ALLEGRO_SAMPLE* winningBGM;
static ALLEGRO_SAMPLE* defeatBGM;
static ALLEGRO_SAMPLE_ID ID_fireball;
static ALLEGRO_SAMPLE_ID BGM;
const int bgConst = 0, bgConst2 = 200; // move to SCREEN_H - this // when y <= this

RecArea buttonArea;
RecArea btMenuArea;
RecArea btRestartArea;
RecArea backbgArea;

static bool is_pausing = false;
static bool is_failed = false;
static bool is_bossing = false;
static bool is_winned = false;
static bool renew = true;
static bool first = true;
static bool winfirst = true;
static bool once_defeat = true;
const float buttonrate = 0.1;

static float dvx, dvy;
static float timer;
static float buttonWidth; // Original
static float buttonHeight; // Original
static float backImgHeight;
static float backImgWidth;
static float bg_x_velocity = 0;
static float bg_y_velocity = -1.5;
static float MyShootingCoolDown = 1.0f;
static float BossShootingCoolDown = 1.25f;

static float playertiemr = 0.0f;
float health;

static int enemy_Num[MAX_ENEMY];
static void init(void) {
	int i;
	renew = first = winfirst = true;
	is_pausing = is_failed = is_bossing = is_winned = false;
	once_defeat = true;
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

	img_plane= load_bitmap(characterlist[MydraIndex][0]);
	int mycoliw = 25, mycolih = 15;
	health = 200.0f * planeAbility[MydraIndex][0] / 2;
	buildplane(&Myplane, img_plane,health, 0, 0, mycoliw, mycolih, MyShootingCoolDown, 0, 0);
	Myplane.Data.x = 400;
	Myplane.Data.y = 500;
	Myplane.sourceX = 0;
	Myplane.sourceY = 3;
	Myplane.Data.hidden = false;
	SCORE = 0;
	
	stageflag = 0;

	BOSS_img = load_bitmap_resized(characterlist[MydraIndex][1],600,600);
	int BOSScoliw = 30, BOSScolih = 30, BOSShealth = BOSS_HEALTH;
	buildplane(&BOSS, BOSS_img,BOSShealth, 3, 0, BOSScoliw, BOSScolih, BossShootingCoolDown,3, 500);
	BOSS.Data.x = SCREEN_W / 2 - al_get_bitmap_width(BOSS_img) / 2;
	BOSS.Data.y = -300;
	BOSS.Data.hidden = true;
	//BOSS_img = load_bitmap(characterlist[MydraIndex][1], 200, 250);
	//for (int i = 0; i < BOSS_SKILL_NUM; i++) {

	//	buildplane(&boss_skill_plane[i],BOSS_img, BOSShealth, 0, 0, BOSScoliw, BOSScolih, BossShootingCoolDown, 3, 0);
	//	boss_skill_plane[i].Data.y = -100;
	//	boss_skill_plane[i].Data.x = -100;
	//}



	img_enemy_bullet = al_load_bitmap("image12.png");
	for (int j = 0; j < ENEMY_TYPES; j++) {
		float health = 100.0f, vx = 0.0f, vy = 0.0f, shootingcooldown = 1.0f;
		int dcollisionw = 0, dcollisionh = 0, shootingBulletType = 1, prize = 10;
		if (j == 0) {
			img_enemy = load_bitmap(enemylist[3]);
			health = 40.0f;
			shootingcooldown = 1.5f;
			shootingBulletType = 1;
			prize = 10;
		}
		else if (j == 1) {
			img_enemy = load_bitmap(enemylist[1]);
			health = 80.0f;
			vx = 2.0f;
			vy =	0.25f;
			shootingcooldown = 5.0f;
			shootingBulletType = 2;
			prize = 25;
		}
		else if (j == 2) {
			dcollisionh = 0;
			dcollisionw = 0;
		}
		for (i = 0; i < MAX_ENEMY; i++) {
			
				buildplane(&enemy_plane[j][i], img_enemy, health, vx, vy,dcollisionw,dcollisionh,shootingcooldown, shootingBulletType, prize);
		}
	}
	for (int i = 0; i < MAX_ENEMY; i++) {
		enemy_plane[1][i].health = 100.0f;
		enemy_plane[1][i].Data.vy = 0.75f;
		enemy_plane[1][i].shootingCoolDown = 2.0f;
		enemy_plane[1][i].shootingBulletType = 2;
	}
	// Initialize bullets.
	for (int j = 0; j < BULLET_TYPES; j++) {
		float damage = 10.0f, vx = 0.0f, vy = 0.0f;
		int dcollisionw = 0, dcollisionh = 0;
		img_bullet = al_load_bitmap(bulletImglist[j]);
		if (j == 0) {
			
			vy = -3.0f;
		}
		else if (j == 1) {
			vy = 3.0f;
		}
		else if (j == 2) {
			vy = 1.5f;
			damage = 15.0f;
		}
		else if (j == 2) {
			vy = 4.0f;
			damage = 25.0f;
		}
		for (int i = 0; i < MAX_BULLET; i++) {
			buildbullet(&bullets[j][i], img_bullet, damage, vx, vy, dcollisionw, dcollisionw);
		}
	}
	// Can be moved to shared_init to decrease loading time.

	fightingBGM = load_audio("Music/Bgm/FightingBGM.ogg");
	BossBGM = load_audio("Music/Bgm/BossBGM.ogg");
	winningBGM = load_audio("Music/Bgm/winningBgm.ogg");
	defeatBGM = load_audio("Music/Bgm/DeathBGM.ogg");
	fireball = load_audio("Music/soundEffect/Fireball/fireball_1_fixed.ogg");
	//bgm = load_audio("mythica.ogg");
	game_log("Start scene initialized");
	stop_bgm(BGM);
	BGM = play_bgm(fightingBGM, VOLUME);
	timer = al_get_time();
}

static void update(void) {
	if (is_pausing)
		return;
	update_BG_area(&backbgArea);
	// My location Update

	Myplane.Data.vx = Myplane.Data.vy = 0;
	if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
		Myplane.Data.vy -= 1;
	if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
		Myplane.Data.vy += 1;
	if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A]) {
		Myplane.Data.vx -= 1;
	}
	if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D]) {
		Myplane.Data.vx += 1;
	}

	Myplane.Data.y += Myplane.Data.vy * 4 * (Myplane.Data.vx ? 0.71f : 1); // 0.71 is (1/sqrt(2)).
	Myplane.Data.x += Myplane.Data.vx * 4 * (Myplane.Data.vy ? 0.71f : 1);
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
	is_bossing = true;
	for (int j = 0; j < ENEMY_TYPES; j++)
		for (int i = 0; i < MAX_ENEMY; i++) {
			
			if (enemy_plane[j][i].Data.hidden)
				continue;
			movOjBounederCheck(&enemy_plane[j][i].Data);
			if (enemy_plane[j][i].health <= 0) {
				enemy_plane[j][i].Data.hidden = true;
				SCORE += enemy_plane[j][i].prize;
				continue;
			}
			
			is_bossing = false;
			changePlaneImg(&enemy_plane[j][i]);
			if (j == 0) {
				smallPlaneScript(&enemy_plane[j][i]);
				shootingScript(&enemy_plane[j][i]);
			}
			else if (j == 1) {
				secondPlaneScript(&enemy_plane[j][i]);
				secondShootScript(&enemy_plane[j][i]);
			}
			changePlanePosition(&enemy_plane[j][i]);
			if (MovableObjectCollision(enemy_plane[j][i].Data, Myplane.Data)) {
				Myplane.health -= 5;
			}
		}
	if (is_bossing) {
		
		if (first) {
			BOSS.Data.y = -300;
			BOSS.Data.vx = 3;
			BOSS.Data.hidden = false;
			first = false;
			stop_bgm(BGM);

			BGM =  play_bgm(BossBGM,VOLUME);
		}
		if (BOSS.health <= 0.0f && winfirst) {
			stop_bgm(BGM);
			BGM = play_bgm(winningBGM,VOLUME);
			BOSS.Data.hidden = true;
			SCORE += BOSS.prize;
			float now = al_get_time();
			SCORE += 20000 / (now - timer);
			is_bossing = false;
			is_winned = true;
			winfirst = false;
		}
		else{
			changePlaneImg(&BOSS);
			BossmovementScript(&BOSS);
			changePlanePosition(&BOSS);
			if (MovableObjectCollision(BOSS.Data, Myplane.Data)) {
				Myplane.health -= 10;
			}
			BossshootingScript(&BOSS);
			//BossSkillScript(&BOSS);
			/*for (int i = 0; i < BOSS_SKILL_NUM; i++) {
				changePlanePosition(&boss_skill_plane[i]);
				changePlaneImg(&boss_skill_plane[i]);
				if (MovableObjectCollision(boss_skill_plane[i].Data, Myplane.Data) ){
					Myplane.health -= 10;
				}
			}*/
		}
	}
	all_bullet_planes_collision_check();
	changePlaneImg(&Myplane);
	double now = al_get_time();
	int i;
	if ((mouse_state[1]) && now - last_shoot_timestamp >= MAX_COOLDOWN) {
		for (i = 0; i < MAX_BULLET; i++) {
			if (bullets[Myplane.shootingBulletType][i].Data.hidden)
				break;
		}
		if (i < MAX_BULLET) {
			last_shoot_timestamp = now;
			bullets[Myplane.shootingBulletType][i].Data.vy = -5;
			bullets[Myplane.shootingBulletType][i].Data.hidden = false;
			bullets[Myplane.shootingBulletType][i].Data.x = Myplane.Data.x;
			bullets[Myplane.shootingBulletType][i].Data.y = Myplane.Data.y - Myplane.Data.h / 2;
			bullets[Myplane.shootingBulletType][i].whose = 1;
			stop_bgm(ID_fireball);
			ID_fireball = play_audio(fireball, VOLUME);
		}
		
			
		
	}

	if (Myplane.health <= 0) {
		Myplane.Data.hidden = true;
		is_failed = true;
		
		if (once_defeat) {
			stop_bgm(BGM);
			BGM = play_bgm(defeatBGM, VOLUME);
			once_defeat = false;
		}
	}

}

static void draw_movable_object(MovableObject obj) {
	if (obj.hidden)
		return;
	al_draw_bitmap(obj.img, round(obj.x - obj.collisionw / 2), round(obj.y - obj.collisionh / 2), 0);
	if (draw_gizmos) {
		al_draw_rectangle(round(obj.x - obj.collisionw / 2), round(obj.y - obj.collisionh / 2),
			round(obj.x + obj.collisionw / 2) + 1, round(obj.y + obj.collisionh / 2) + 1, al_map_rgb(255, 0, 0), 0);
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
	drawplane(&Myplane);
	
	for (j = 0; j < ENEMY_TYPES; j++) {
		for (i = 0; i < MAX_ENEMY; i++) {
			drawplane(&enemy_plane[j][i]);
		}
	}
	if (is_bossing) {
		drawplane(&BOSS);
		//for (int i = 0; i < BOSS_SKILL_NUM; i++)
			//drawplane(&boss_skill_plane[i]);
		if (BOSS.health > 0) {
			float BH = BOSS_HEALTH;
		    al_draw_filled_rectangle(200, 10, 200  +  800 * BOSS.health / BH , 35,al_map_rgb(255,0,0));
			al_draw_rectangle(195, 5, 1005, 40, al_map_rgb(0, 0, 102), 10);
		}
	}

	drawSelfHealth();
	if(!is_winned && !is_failed)
		drawSCORE();
	if (is_pausing || is_failed || is_winned) {
		al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(64, 64, 64, 200));
		al_draw_filled_rectangle(btMenuArea.x, btMenuArea.y, btMenuArea.x2, btMenuArea.y2, al_map_rgb(51, 51, 255));
		al_draw_text(font_pirulen_24, al_map_rgb(0, 25, 51), btMenuArea.x + 25, btMenuArea.y + 5, 0, "back to menu");
		al_draw_filled_rectangle(btRestartArea.x, btRestartArea.y, btRestartArea.x2, btRestartArea.y2, al_map_rgb(54, 23, 167));
		al_draw_text(font_pirulen_24, al_map_rgb(132, 54, 34), btRestartArea.x + 25, btRestartArea.y + 5, 0, "Restart");
	}
	if (is_failed) {
		//al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(64, 64, 64, 200));
		al_draw_text(font_pirulen_32, al_map_rgb(255, 0, 0), SCREEN_W / 2 - 100, SCREEN_H / 2 - 150, 0, "DEFEAT");
	}
	else if (is_winned)
		al_draw_text(font_pirulen_32, al_map_rgb(0, 0, 255), SCREEN_W / 2 - 100, SCREEN_H / 2 - 150, 0, "VICTORY");

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
		if (is_winned || is_failed) {
			al_draw_textf(font_pirulen_24, al_map_rgb(0, 0, 0), 200, 200, 0, "Your Score: %d", SCORE);
			if (renew) {
				FILE* tmp;
				tmp = fopen("Record/score_record.txt", "a+");
				fprintf(tmp, " %d", SCORE);
				fclose(tmp);
				renew_Record();
				renew = false;
			}
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
	al_destroy_bitmap(originBitmap);
	al_destroy_bitmap(BOSS_img);
	al_destroy_sample(fireball);
	al_destroy_sample(fightingBGM);
	al_destroy_sample(BossBGM);
	al_destroy_sample(winningBGM);
	al_destroy_sample(defeatBGM);

	//  Destroy your bullet image.
	stop_bgm(BGM);
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
	/*if (is_winned) {
		
	}*/
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




void drawplane(planeObject* plane) {
	if (plane->Data.hidden)
		return;
	al_draw_bitmap_region(plane->Data.img,
		plane->sourceX * plane->Data.w,
		plane->sourceY * plane->Data.h,
		plane->Data.w,
		plane->Data.h,
		plane->Data.x - plane->Data.w/2,
		plane->Data.y - plane->Data.h/2,
		0);
	al_draw_tinted_bitmap_region(plane->Data.img,al_map_rgba(0,0,0,50),
		plane->sourceX * plane->Data.w,
		plane->sourceY * plane->Data.h,
		plane->Data.w,
		plane->Data.h,
		plane->Data.x - plane->Data.w / 2 + 50,
		plane->Data.y - plane->Data.h / 2 + 50,
		0);
	if (draw_gizmos) {
		MovableObject obj = plane->Data;
		al_draw_rectangle(round(obj.x - obj.collisionw / 2), round(obj.y - obj.collisionh / 2),
			round(obj.x + obj.collisionw / 2) + 1, round(obj.y + obj.collisionh / 2) + 1, al_map_rgb(255, 0, 0), 0);
	}
}
void selfBounderCheck() {
	// Limit the plane's collision box inside the frame.
	if (Myplane.Data.x < 0 + Myplane.Data.w / 2)
		Myplane.Data.x = Myplane.Data.w / 2;
	else if (Myplane.Data.x > (SCREEN_W - Myplane.Data.w / 2))
		Myplane.Data.x = SCREEN_W - Myplane.Data.w / 2;
	if (Myplane.Data.y < 0 + Myplane.Data.h / 2)
		Myplane.Data.y = Myplane.Data.h / 2;
	else if (Myplane.Data.y > (SCREEN_H - Myplane.Data.h / 2))
		Myplane.Data.y = SCREEN_H - Myplane.Data.h / 2;
}

void drawSelfHealth() {
	if (Myplane.health > 0) {
		al_draw_filled_rectangle(10, SCREEN_H - 40, 10 + 300  *  Myplane.health / health, SCREEN_H - 10, al_map_rgb(51, 255, 51));
		al_draw_rectangle(15, SCREEN_H - 35, 15 +  300, SCREEN_H - 5, al_map_rgb(51, 0, 102),10);

	}
	
}
void drawSCORE() {
	al_draw_textf(font_pirulen_32, al_map_rgb(127, 0, 255), SCREEN_W -150, 10, 0, "%d", SCORE);
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
							/*al_draw_tinted_bitmap_region(enemy_plane[k][q].Data.img, al_map_rgba(255, 0, 0, 100),
								enemy_plane[k][q].sourceX * enemy_plane[k][q].Data.w,
								enemy_plane[k][q].sourceY * enemy_plane[k][q].Data.h,
								enemy_plane[k][q].Data.w,
								enemy_plane[k][q].Data.h,
								enemy_plane[k][q].Data.x - enemy_plane[k][q].Data.w / 2,
								enemy_plane[k][q].Data.y - enemy_plane[k][q].Data.h / 2,
								0);*/
						}
					}
				}
				if (MovableObjectCollision(bullets[j][i].Data, BOSS.Data)) {
					BOSS.health -= bullets[j][i].damage;
					game_log("%f", BOSS.health);
					bullets[j][i].Data.hidden = true;
				}
			}
			else {
				if (MovableObjectCollision(bullets[j][i].Data, Myplane.Data)) {
					Myplane.health -= bullets[j][i].damage;
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