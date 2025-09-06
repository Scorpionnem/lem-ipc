/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:28:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/06 22:47:55 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"
#include "ctx.h"

int	g_running = 1;

void	handle_sigint(int sig)
{
	(void)sig;
	g_running = 0;
}

static void	move_to(t_ctx *ctx, int target_x, int target_y)
{
	if (target_x < 0 || target_x >= BOARD_SIZE
		|| target_y < 0 || target_y >= BOARD_SIZE)
		return ;
	if (ctx->shm->board[target_y][target_x] == 0)
	{
		ctx->shm->board[target_y][target_x] = ctx->team;
		ctx->shm->board[ctx->pos_y][ctx->pos_x] = 0;
		ctx->pos_x = target_x;
		ctx->pos_y = target_y;
	}
}

bool	is_walkable(t_ctx *ctx, int x, int y)
{
	int	point;

	point = 0;
	if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE)
	{
		point = ctx->shm->board[y][x];
		if (point == 0)
			return (true);
	}
	return (false);
}

int heuristic(int x, int y, int target_x, int target_y)
{
	return 10 * (abs(target_x - x) + abs(target_y - y));
}

int	find_path(t_ctx *ctx, int target_x, int target_y)
{
	int	smallest = __INT_MAX__;
	int	dir = 0;
	int	pos_x;
	int	pos_y;

	pos_x = ctx->pos_x + 1;
	pos_y = ctx->pos_y;
	if (heuristic(pos_x, pos_y, target_x, target_y) < smallest
		&& is_walkable(ctx, pos_x, pos_y))
	{
		dir = 1;
		smallest = heuristic(pos_x, pos_y, target_x, target_y);
	}
	pos_x = ctx->pos_x;
	pos_y = ctx->pos_y + 1;
	if (heuristic(pos_x, pos_y, target_x, target_y) < smallest
		&& is_walkable(ctx, pos_x, pos_y))
	{
		dir = 2;
		smallest = heuristic(pos_x, pos_y, target_x, target_y);
	}
	pos_x = ctx->pos_x - 1;
	pos_y = ctx->pos_y;
	if (heuristic(pos_x, pos_y, target_x, target_y) < smallest
		&& is_walkable(ctx, pos_x, pos_y))
	{
		dir = 3;
		smallest = heuristic(pos_x, pos_y, target_x, target_y);
	}
	pos_x = ctx->pos_x;
	pos_y = ctx->pos_y - 1;
	if (heuristic(pos_x, pos_y, target_x, target_y) < smallest
		&& is_walkable(ctx, pos_x, pos_y))
	{
		dir = 4;
		smallest = heuristic(pos_x, pos_y, target_x, target_y);
	}
	return (dir);
}

bool	is_in_bounds(t_ctx *ctx, int x, int y)
{
	int	point;

	point = 0;
	if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE)
	{
		point = ctx->shm->board[y][x];
		if (point != ctx->team && point != 0)
			return (true);
	}
	return (false);
}

int	is_dead(t_ctx *ctx)
{
	int	neighbours;

	neighbours = 0;
	neighbours += is_in_bounds(ctx, ctx->pos_x + 1, ctx->pos_y + 1);
	neighbours += is_in_bounds(ctx, ctx->pos_x - 1, ctx->pos_y - 1);
	neighbours += is_in_bounds(ctx, ctx->pos_x + 1, ctx->pos_y - 1);
	neighbours += is_in_bounds(ctx, ctx->pos_x - 1, ctx->pos_y + 1);
	neighbours += is_in_bounds(ctx, ctx->pos_x + 1, ctx->pos_y);
	neighbours += is_in_bounds(ctx, ctx->pos_x, ctx->pos_y + 1);
	neighbours += is_in_bounds(ctx, ctx->pos_x - 1, ctx->pos_y);
	neighbours += is_in_bounds(ctx, ctx->pos_x, ctx->pos_y - 1);
	if (neighbours >= DEATH_THRESHOLD)
		return (1);
	return (0);
}

t_gameinfo	get_game_infos(t_ctx *ctx);

void	move(t_ctx *ctx)
{
	int	target_x = 10;
	int	target_y = 10;

	if (ctx->pos_x != target_x || ctx->pos_y != target_y)
	{
		int dir = find_path(ctx, target_x, target_y);
		if (dir == 1)
			move_to(ctx, ctx->pos_x + 1, ctx->pos_y);
		if (dir == 2)
			move_to(ctx, ctx->pos_x, ctx->pos_y + 1);
		if (dir == 3)
			move_to(ctx, ctx->pos_x - 1, ctx->pos_y);
		if (dir == 4)
			move_to(ctx, ctx->pos_x, ctx->pos_y - 1);
	}
}

void	game_loop(t_ctx *ctx)
{
	if (ctx->shm->paused)
		return ;

	t_gameinfo	infos;

	infos = get_game_infos(ctx);
	if (is_dead(ctx) || infos.teams == 1)
	{
		g_running = false;
		return ;
	}

	move(ctx);
}

int	main(int ac, char **av)
{
	t_ctx	ctx;

	signal(SIGINT, handle_sigint);
	if (!init_ctx(&ctx, ac, av))
		return (1);
	while (g_running)
	{
		sem_lock(ctx.semid);
		game_loop(&ctx);
		sem_unlock(ctx.semid);
		usleep(SLEEP_TIME);
	}
	return (delete_ctx(&ctx));
}
