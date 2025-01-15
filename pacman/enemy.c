#include "pacman.h"
#include <stdbool.h>
#include <limits.h>

// ===== A* IMPLEMENTATION ===== //

#define GRID_SIZE GRID_HEIGHT * GRID_WIDTH

typedef struct {
	int x, y;
	int cost, heuristic;
	int total_cost; 				// F(n) = G(n) + H(n)
	dir_t direction;
} node_t;

static int manhattan_distance(int x1, int y1, int x2, int y2) {
	int dx = (x1 > x2) ? (x1 - x2) : (x2 - x1);
	int dy = (y1 > y2) ? (y1 - y2) : (y2 - y1);
	return dx + dy;
}

node_t open_set[GRID_SIZE];
bool closed_set[GRID_HEIGHT][GRID_WIDTH] = {false};

static void reset_globals() {
	for (int i = 0; i < GRID_SIZE; i++) {
		open_set[i] = (node_t){0, 0, 0, 0, INT_MAX, STOP};
	}
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			closed_set[y][x] = false;
		}
	}
}

static dir_t a_star(game_t* game, cell_t grid[GRID_HEIGHT][GRID_WIDTH], int enemy_num) {
	reset_globals();
	
	int start_x = game->enemy->enemy_x[enemy_num];
	int start_y = game->enemy->enemy_y[enemy_num];
	
	int target_x;
	int target_y;
	
	// reverse target if frightened
	if(game->enemy->enemy_fright){
		target_x = GRID_WIDTH - game->player->player_x;
		target_y = GRID_HEIGHT - game->player->player_y;
	} else {
		target_x = game->player->player_x;
		target_y = game->player->player_y;
	}

	int open_set_size = 0;

	dir_t directions[4] = {UP, DOWN, LEFT, RIGHT};
	int dx[4] = {0, 0, -1, 1};
	int dy[4] = {-1, 1, 0, 0};

	// starting node
	open_set[open_set_size++] = (node_t){start_x, start_y, 0, manhattan_distance(start_x, start_y, target_x, target_y), 0, STOP};

	while (open_set_size > 0) {
		// get node with the lowest cost
		int best_index = 0;
		for (int i = 1; i < open_set_size; i++) {
			if (open_set[i].total_cost < open_set[best_index].total_cost) {
				best_index = i;
			}
		}

		node_t current = open_set[best_index];
		open_set[best_index] = open_set[--open_set_size];

		// Mark current as visited
		closed_set[current.y][current.x] = true;

		// if target reached
		if (current.x == target_x && current.y == target_y) {
			return current.direction;
		}

		// explore neighbors
		for (int i = 0; i < 4; i++) {
			int neighbor_x = current.x + dx[i];
			int neighbor_y = current.y + dy[i];

			// Skip invalid or visited nodes
			if (neighbor_x < 0 || neighbor_y < 0 || neighbor_x >= GRID_WIDTH || neighbor_y >= GRID_HEIGHT ||
				grid[neighbor_y][neighbor_x] == HOR_WALL || grid[neighbor_y][neighbor_x] == VER_WALL ||
				closed_set[neighbor_y][neighbor_x]) {
				continue;
			}

			int g_cost = current.cost + 1; // Uniform cost for moving one cell
			int h_cost = manhattan_distance(neighbor_x, neighbor_y, target_x, target_y);
			int total_cost = g_cost + h_cost;

			// Check if this neighbor is already in the open set with a better cost
			bool in_open_set = false;
			for (int j = 0; j < open_set_size; j++) {
				if (open_set[j].x == neighbor_x && open_set[j].y == neighbor_y) {
					in_open_set = true;
					if (total_cost < open_set[j].total_cost) {
						open_set[j].cost = g_cost;
						open_set[j].total_cost = total_cost;
						open_set[j].direction = current.direction == STOP ? directions[i] : current.direction;
					}
					break;
				}
			}
			// Add the neighbor to the open set if not already present
			if (!in_open_set) {
				open_set[open_set_size++] = (node_t){
					neighbor_x, neighbor_y, g_cost, h_cost, total_cost,
					current.direction == STOP ? directions[i] : current.direction};
			}
		}
	}
	return STOP;
}

void calc_enemy_dir(cell_t grid[GRID_HEIGHT][GRID_WIDTH], game_t* game){
	dir_t dir = STOP;
	for(int i=0; i<ENEMY_NUM; i++){
		// only non blocked enemies can move
		if(game->enemy->respawn[i]){
			// if still inside prison
			if(game->enemy->enemy_y[i] >= 13 && game->enemy->enemy_y[i] <= 15 &&  game->enemy->enemy_x[i] >= 11 && game->enemy->enemy_x[i] <= 16){
				// Just go up since spawn point static
				dir = UP;
			}
			else {
				dir = a_star(game, grid, i);
			}
			game->enemy->edir[i] = dir;
		}
	}
}