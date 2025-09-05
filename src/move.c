/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:25:53 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 18:44:07 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	move_to(t_ctx *ctx, int target_x, int target_y)
{
	if (target_x < 0 || target_x >= BOARD_SIZE
		|| target_y < 0 || target_y >= BOARD_SIZE)
		return ;
	if (ctx->shared_memory->board[target_y][target_x] == 0)
	{
		ctx->shared_memory->board[target_y][target_x] = ctx->team;
		ctx->shared_memory->board[ctx->pos_y][ctx->pos_x] = 0;
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
		point = ctx->shared_memory->board[y][x];
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

void	find_target(t_ctx *ctx)
{
	int	dist;
	int	sx;
	int	sy;

	sx = BOARD_SIZE / 2;
	sy = BOARD_SIZE / 2;
	dist = __INT_MAX__;
	for (int y = rand() % BOARD_SIZE; y < BOARD_SIZE; y++)
		for (int x = rand() % BOARD_SIZE; x < BOARD_SIZE; x++)
		{
			if (ctx->shared_memory->board[y][x] != ctx->team && heuristic(ctx->pos_x, ctx->pos_y, x, y) < dist)
			{
				dist = heuristic(ctx->pos_x, ctx->pos_y, x, y);
				sx = x;
				sy = y;
			}
		}
	ctx->target_x = sx;
	ctx->target_x = sy;
}

void	move(t_ctx *ctx)
{
	int	dir;

	if (ctx->is_team_leader)
		find_target(ctx);

	if (ctx->pos_x == ctx->target_x && ctx->pos_y == ctx->target_y)
		return ;

	dir = find_path(ctx, ctx->target_x, ctx->target_y);
	if (dir == 1)
		move_to(ctx, ctx->pos_x + 1, ctx->pos_y);
	if (dir == 2)
		move_to(ctx, ctx->pos_x, ctx->pos_y + 1);
	if (dir == 3)
		move_to(ctx, ctx->pos_x - 1, ctx->pos_y);
	if (dir == 4)
		move_to(ctx, ctx->pos_x, ctx->pos_y - 1);
}
