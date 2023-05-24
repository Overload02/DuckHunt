#include "ui.h"
#include "ducks.h"
#include "dog.h"
#include "menu.h"
#include "player.h"
#include "gfx/DUCKSPT.h"

/**
 * This is used to draw the scene for the game.
 * It depends on the me variable (menu.opt).
 */
void draw_scene(void)
{
	gfx_sprite_t *back_buffer, *temp;

	gfx_FillScreen(2); // BLACK

	gfx_SetColor(1); // WHITE
	gfx_Rectangle(31, 7, 258, 225);

	// Renders Sky
	switch (menu.opt)
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

	if (menu.opt < 3)
	{ // Game A & Game B
		// tree
		temp = gfx_MallocSprite(bg_tree_width, bg_tree_height);
		zx7_Decompress(temp, bg_tree_compressed);
		gfx_TransparentSprite(temp, 44, 39);

		// bush
		temp = gfx_MallocSprite(bg_bush_width, bg_bush_height);
		zx7_Decompress(temp, bg_bush_compressed);
		gfx_TransparentSprite(temp, 218, 132);

		// ground
		gfx_SetColor(6);
		gfx_FillRectangle(32, 190, 256, 41);

		temp = gfx_MallocSprite(bg_ground_width, bg_ground_height);
		zx7_Decompress(temp, bg_ground_compressed);
		gfx_TransparentSprite(temp, 32, 156);

		back_buffer = gfx_MallocSprite(1, bg_ground_height);
		gfx_GetSprite(back_buffer, 286, 156);
		gfx_Sprite(back_buffer, 287, 156);
		free(back_buffer);

		// bullet board
		temp = gfx_MallocSprite(bullet_board_width, bullet_board_height);
		zx7_Decompress(temp, bullet_board_compressed);
		gfx_TransparentSprite(temp, 52, 209);

		// kill board
		temp = gfx_MallocSprite(kill_board_width, kill_board_height);
		zx7_Decompress(temp, kill_board_compressed);
		gfx_TransparentSprite(temp, 92, 209);

		// score board
		temp = gfx_MallocSprite(score_board_width, score_board_height);
		zx7_Decompress(temp, score_board_compressed);
		gfx_TransparentSprite(temp, 220, 209);

		free(temp);
	}
	else
	{ // Game C
	}

	gfx_Blit(1);
}

/**
 * This is used to innit the game and setup and variables.
 */
void init_duckhunt(uint8_t type)
{
	/* Initialize the player position */
	player.x = 320 / 2;
	player.y = 240 / 2;
	player.score = 0;
	player.speed = 5;
	player.level = 1;

	/* Makes sure that the type is only game a or b */
	if (type < 3)
	{
		dog_SetMode(0);
	}

	/* Set the enemies types */
	switch (type)
	{
	case 1:
		init_enemies(1, 1);
		break;

	case 2:
		init_enemies(2, 3);
		break;

	case 3:
		init_enemies(2, 1);
		break;
	}
}

/**
 * This is a function used for displaying a version number.
 */
void show_ver(void)
{
	gfx_SetTextFGColor(1);
	gfx_SetTextXY(1, 230);

	gfx_PrintString(DUCKHUNT_VERSION);
}

// New functions to help with rendering.
/**
 * This function grabs buffer sprite.
 */
void get_buffer_layer(void)
{
	/* Allocate space for back buffer */
	player.back_buffer = gfx_MallocSprite(crosshair_width, crosshair_height);

	/* Grab player back buffer */
	gfx_GetSprite(player.back_buffer, player.x - 12, player.y - 12);

	for (int i = 0; i < enemies_amount; i++)
	{
		/* Allocate space for back buffer */
		enemies[i].back_buffer = gfx_MallocSprite(38, 38);

		/* Grab duck back buffer */
		gfx_GetSprite(enemies[i].back_buffer, enemies[i].x - 1, enemies[i].y - 1);
	}
}

/**
 * This draws any sprite buffered from "get_buffer_layer"
 */
void draw_buffer_layer(void)
{
	/* Render player back buffer*/
	gfx_Sprite(player.back_buffer, player.x - 12, player.y - 12);

	/* Render duck back buffer */
	draw_duck_buffer_layer();
}

void draw_duck_buffer_layer(void)
{
	for (int i = 0; i < enemies_amount; i++)
	{
		gfx_TransparentSprite(enemies[i].back_buffer, enemies[i].x - 1, enemies[i].y - 1);
	}
}

/**
 * @brief Free the sprite from buffers
 *
 */
void free_buffer_layer(void)
{
	/* Free player back buffer */
	free(player.back_buffer);

	/* Free duck back buffer */
	for (int i = 0; i < enemies_amount; i++)
	{
		free(enemies[i].back_buffer);
	}
}
