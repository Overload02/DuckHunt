#include "ui.h"
#include "utils.h"
#include "ducks.h"
#include "menu.h"
#include "dog.h"
#include "player.h"
#include "gfx/DUCKSPT.h"

struct game_t game;

/* https://www.geeksforgeeks.org/bubble-sort/# */
static void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void bubble_sort_hits_panel(void)
{
	int i, j;
	bool swapped;

	for (i = 0; i < 10 - 1; i++)
	{
		swapped = false;
		for (j = 0; j < 10 - i - 1; j++)
		{
			if (game.duck_hits[j] > game.duck_hits[j + 1])
			{
				swap(&game.duck_hits[j], &game.duck_hits[j + 1]);
				swapped = true;

				update_scene();
				gfx_Blit(1);
				delay(50);
			}
		}

		// If no two elements were swapped by inner loop,
		// then break
		if (swapped == false)
			break;
	}

	delay(300);
}

/* Used to get the amount of shot ducks based off the HUD */
int get_hud_hits_shot(void)
{
	int tick = 0;

	for (int i = 0; i < 10; i++)
	{
		if (game.duck_hits[i] == DUCK_SHOT)
		{
			tick++;
		}
	}

	return tick;
}

static void flyaway_scene(void)
{
	gfx_sprite_t *temp = gfx_MallocSprite(fly_away_width, fly_away_height);
	gfx_sprite_t *back_buff = gfx_MallocSprite(fly_away_width, fly_away_height);

	gfx_GetSprite(back_buff, 124, 103);

	zx7_Decompress(temp, fly_away_compressed);
	gfx_TransparentSprite(temp, 124, 103);
	free(temp);

	gfx_Blit(1);

	gfx_Sprite(back_buff, 124, 103);
	free(back_buff);
}

static void pause_scene(void)
{
	gfx_sprite_t *temp = gfx_MallocSprite(pause_width, pause_height);
	gfx_sprite_t *back_buff = gfx_MallocSprite(pause_width, pause_height);

	gfx_GetSprite(back_buff, 135, 88);

	/* Pretend this is the dog */
	/* gfx_SetColor(1); // White
	gfx_FillRectangle(135, 88, 49, 33); */

	zx7_Decompress(temp, pause_compressed);
	gfx_TransparentSprite(temp, 135, 88);
	free(temp);

	gfx_Blit(1);

	delay(60);

	while (!os_GetCSC())
		;

	delay(60);

	gfx_Sprite(back_buff, 135, 88);
	free(back_buff);
}

void update_scene(void)
{
	gfx_sprite_t *temp;

	// Draw Bullets
	gfx_SetColor(2); // Black
	if (menu.option < 3)
	{
		gfx_FillRectangle(57, 212, 20, 7);
	}
	else
	{
		gfx_FillRectangle(57, 204, 20, 7);
	}

	temp = gfx_MallocSprite(bullet_width, bullet_height);
	zx7_Decompress(temp, bullet_compressed);

	for (int i = 0; i < player.bullets; i++)
	{

		if (menu.option < 3)
		{
			// Game A and B
			gfx_TransparentSprite(temp, 57 + (i * (4 + 4)), 212);
		}
		else
		{
			// Game C
			gfx_TransparentSprite(temp, 57 + (i * (4 + 4)), 204);
		}
	}

	free(temp);

	/* Hit Panel */

	temp = gfx_MallocSprite(panel_icon_1_width, panel_icon_1_height);

	for (int i = 0; i < 10; i++)
	{
		if (menu.option < 3)
		{
			// Game A and B
			if (game.duck_hits[i] == DUCK_SHOT)
			{
				zx7_Decompress(temp, panel_icon_2_compressed);
			}
			else
			{
				zx7_Decompress(temp, panel_icon_1_compressed);
			}

			gfx_TransparentSprite(temp, 127 + (i * (panel_icon_1_width + 1)), 213);

			/* Blinking */
			if (dog.mode != DOG_RUN_TO_CENTER && game.start == true)
			{
				if (get_hud_hits_position() == i)
				{
					if (randInt(0, 1))
					{
						gfx_SetColor(2);
						gfx_FillRectangle(127 + (i * (8)), 213, 7, 7);
					}
				}
			}
		}
		else
		{

			if (game.duck_hits[i])
			{
				zx7_Decompress(temp, panel_icon_4_compressed);
				gfx_SetColor(16);
			}
			else
			{
				zx7_Decompress(temp, panel_icon_5_compressed);
				gfx_SetColor(1);
			}

			gfx_TransparentSprite(temp, 127 + (i * (panel_icon_1_width + 1)), 205);

			if (game.start == true)
			{
				if (get_hud_hits_position() == i)
				{
					if (randInt(0, 1))
					{
						gfx_SetColor(2);
						gfx_FillRectangle(127 + (i * (8)), 205, 7, 7);
					}
				}
			}
		}
	}

	free(temp);

	/* Advanced amount */
	if (player.round >= 1 && player.round <= 10)
	{
		GAME_ADVANCE_THRESHOLD = 6;
	}
	else if (player.round >= 11 && player.round <= 12)
	{
		GAME_ADVANCE_THRESHOLD = 7;
	}
	else if (player.round >= 13 && player.round <= 14)
	{
		GAME_ADVANCE_THRESHOLD = 8;
	}
	else if (player.round >= 15 && player.round <= 19)
	{
		GAME_ADVANCE_THRESHOLD = 9;
	}
	else
	{
		GAME_ADVANCE_THRESHOLD = 10;
	}

	temp = gfx_MallocSprite(panel_icon_3_width, panel_icon_3_height);
	zx7_Decompress(temp, panel_icon_3_compressed);

	for (int i = 0; i < GAME_ADVANCE_THRESHOLD; i++)
	{
		if (menu.option < 3)
		{
			gfx_TransparentSprite(temp, 127 + (i * (panel_icon_3_width + 1)), 221);
		}
		else
		{
			gfx_TransparentSprite(temp, 127 + (i * (panel_icon_3_width + 1)), 213);
		}
	}

	free(temp);

	// Draw Score

	/* Fill the area up */
	gfx_SetColor(2);	   // Black
	gfx_SetTextFGColor(1); // White

	if (menu.option < 3)
	{
		gfx_FillRectangle(223, 211, 47, 8);

		gfx_SetTextXY(223, 211);
		gfx_PrintInt(player.score, 6);
	}
	else
	{
		gfx_FillRectangle(223, 203, 47, 8);

		gfx_SetTextXY(223, 203);
		gfx_PrintInt(player.score, 6);
	}

	// Draw Round
	gfx_SetTextFGColor(16); // Green
	gfx_SetTextBGColor(2);	// Black
	gfx_SetTextXY(253, 11);
	gfx_PrintString("R=");
	gfx_PrintInt(player.round, 0);

	/* Render Fly away Scene for game A and B */
	if (menu.option < 3)
	{
		if (player.bullets == 0)
		{
			// Check if there are any birds on scene
			if (DUCK_FALLEN_AMOUNT != DUCK_AMOUNT)
			{
				flyaway_scene();
			}
		}
	}

	if (kb_Data[1] == kb_Mode)
	{
		pause_scene();
	}
}

static int count_digits(int number)
{
	int count = 0;
	while (number > 0)
	{
		number /= 10;
		count++;
	}
	return count;
}

/**
 * This is used to draw the scene for the game.
 * It depends on the me variable (menu.option).
 */
void draw_scene(void)
{
	gfx_sprite_t *temp;

	game.start = false;
	DUCK_FLYAWAY_TIMER = 0;

	gfx_FillScreen(2); // BLACK

	gfx_SetColor(1); // WHITE
	gfx_Rectangle(31, 7, 258, 225);

	// Renders Sky
	switch (menu.option)
	{
	case 1:
		gfx_SetColor(4);
		gfx_FillRectangle(32, 8, 256, 223);
		break;

	case 2:
		gfx_SetColor(5);
		gfx_FillRectangle(32, 8, 256, 223);
		break;

	case 3:
		gfx_SetColor(3);
		gfx_FillRectangle(32, 8, 256, 223);
		break;
	}

	if (menu.option < 3)
	{ // Game A & Game B
		// tree
		temp = gfx_MallocSprite(bg_tree_width, bg_tree_height);
		zx7_Decompress(temp, bg_tree_compressed);
		gfx_TransparentSprite(temp, 44, 39);
		free(temp);

		// bush
		temp = gfx_MallocSprite(bg_bush_width, bg_bush_height);
		zx7_Decompress(temp, bg_bush_compressed);
		gfx_TransparentSprite(temp, 218, 132);
		free(temp);

		// ground
		gfx_SetColor(6);
		gfx_FillRectangle(32, 190, 256, 41);

		temp = gfx_MallocSprite(bg_ground_width, bg_ground_height);
		zx7_Decompress(temp, bg_ground_compressed);
		gfx_TransparentSprite(temp, 32, 156);
		free(temp);

		// Duplicated Last line
		temp = gfx_MallocSprite(1, bg_ground_height);
		gfx_GetSprite(temp, 286, 156);
		gfx_Sprite(temp, 287, 156);
		free(temp);

		// bullet board
		temp = gfx_MallocSprite(bullet_board_width, bullet_board_height);
		zx7_Decompress(temp, bullet_board_compressed);
		gfx_TransparentSprite(temp, 52, 209);
		free(temp);

		// kill board
		temp = gfx_MallocSprite(kill_board_width, kill_board_height);
		zx7_Decompress(temp, kill_board_compressed);
		gfx_TransparentSprite(temp, 92, 209);
		free(temp);

		// score board
		temp = gfx_MallocSprite(score_board_width, score_board_height);
		zx7_Decompress(temp, score_board_compressed);
		gfx_TransparentSprite(temp, 220, 209);
		free(temp);

		// Print Score Board Information
		update_scene();

		// Print Round Number
		gfx_sprite_t *back_buff = gfx_MallocSprite(fly_away_width, fly_away_height * 2);

		gfx_GetSprite(back_buff, (LCD_WIDTH - fly_away_width) / 2, 103);

		ui_rectangle((LCD_WIDTH - fly_away_width) / 2, 103, fly_away_width, fly_away_height * 2);

		gfx_SetTextFGColor(1);
		gfx_SetTextXY((LCD_WIDTH - gfx_GetStringWidth("ROUND")) / 2, 110);
		gfx_PrintString("ROUND");

		gfx_SetTextXY((LCD_WIDTH - (8 * count_digits(player.round))) / 2, 123);
		gfx_PrintInt(player.round, 0);

		gfx_Blit(1);

		delay(1000);

		/* Print round number back buffer */
		gfx_Sprite(back_buff, (LCD_WIDTH - fly_away_width) / 2, 103);
		free(back_buff);

		// Render dog
		dog_SetMode(DOG_RUN_TO_CENTER);
		draw_dog_scene();
	}
	else
	{ // Game C

		temp = gfx_MallocSprite(c_mountain_width, c_mountain_height);
		zx7_Decompress(temp, c_mountain_compressed);
		gfx_TransparentSprite(temp, 32, 95);
		free(temp);

		temp = gfx_MallocSprite(c_ground_width, c_ground_height);
		zx7_Decompress(temp, c_ground_compressed);
		gfx_TransparentSprite(temp, 32, 140);
		free(temp);

		gfx_SetColor(16); // Green
		gfx_FillRectangle(32, 183, c_panel_width, c_panel_height);

		temp = gfx_MallocSprite(c_panel_width, c_panel_height);
		zx7_Decompress(temp, c_panel_compressed);
		gfx_TransparentSprite(temp, 32, 183);
		free(temp);

		temp = gfx_MallocSprite(1, c_panel_height + c_ground_height + c_mountain_height);
		gfx_GetSprite(temp, 286, 95);
		gfx_Sprite(temp, 287, 95);
		free(temp);

		gfx_SetColor(7); // Gray
		gfx_FillRectangle(32, 191, 256, 40);

		// bullet board
		temp = gfx_MallocSprite(bullet_board_width, bullet_board_height);
		zx7_Decompress(temp, bullet_board_compressed);
		gfx_TransparentSprite(temp, 52, 201);
		free(temp);

		// kill board
		temp = gfx_MallocSprite(kill_board_width, kill_board_height);
		zx7_Decompress(temp, kill_board_compressed);
		gfx_TransparentSprite(temp, 92, 201);
		free(temp);

		// score board
		temp = gfx_MallocSprite(score_board_width, score_board_height);
		zx7_Decompress(temp, score_board_compressed);
		gfx_TransparentSprite(temp, 220, 201);
		free(temp);

		// Print Score Board Information
		update_scene();
	}

	game.start = true;

	gfx_Blit(1);
}

/**
 * This is used to innit the game and setup and variables.
 */
void init_duckhunt(void)
{
	/* Initialize the player position */
	player.x = 320 / 2;
	player.y = 240 / 2;
	player.score = 0;
	player.speed = 5;
	player.level = 1;
	player.round = 1;
	player.bullets = 3;

	/* HUD setup */
	reset_hud_hits();

	/* Dog setup */
	dog_SetMode(DOG_HIDDEN);

	/* Setup the enemies */
	DUCK_FALLEN_AMOUNT = 0;
	GAME_TOTAL_HITS = 0;

	switch (menu.option)
	{
	case 1:
		init_enemies(1);
		break;

	case 2:
	case 3:
		init_enemies(2);
		break;
	}
}

/* HUD UPDATING */
void reset_hud_hits(void)
{
	for (int i = 0; i < 10; i++)
		game.duck_hits[i] = 0;
}

int get_hud_hits_position(void)
{
	int tick = 0;

	for (int i = 0; i < 10; i++)
	{
		if (game.duck_hits[i] == DUCK_SHOT || game.duck_hits[i] == DUCK_FLYAWAY || game.duck_hits[i] == DUCK_FORCED_FLYAWAY)
		{
			tick++;
		}
	}

	return tick;
}

// New functions to help with rendering.
void get_buffer_layer(void)
{
	get_player_buffer_layer();
	get_duck_buffer_layer();

	if (dog.mode != DOG_HIDDEN)
		get_dog_buffer_layer();
}

void draw_buffer_layer(void)
{
	draw_player_buffer_layer();
	draw_duck_buffer_layer();

	if (dog.mode != DOG_HIDDEN)
		draw_dog_buffer_layer();
}