/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 10:34:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/07 12:00:45 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static void	move_to(t_ctx *ctx, int target_x, int target_y)
{
	if (target_x < 0 || target_x >= BOARD_SIZE
		|| target_y < 0 || target_y >= BOARD_SIZE)
		return ;
	if (ctx->shm->board[target_y][target_x] == 0)
	{
		ctx->shm->board[target_y][target_x] = ctx->team;
		ctx->shm->board[ctx->pos_y][ctx->pos_x] = 0;
		ctx->prev_pos_x = ctx->pos_x;
		ctx->prev_pos_y = ctx->pos_y;
		ctx->pos_x = target_x;
		ctx->pos_y = target_y;
	}
}

static bool	is_walkable(t_ctx *ctx, int x, int y)
{
	int	point;

	point = 0;
	if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE)
	{
		point = ctx->shm->board[y][x];
		if (point == 0 && !(x == ctx->prev_pos_x && y == ctx->prev_pos_y))
			return (true);
	}
	return (false);
}

#include <math.h>

int distance(int x, int y, int target_x, int target_y)
{
	return (sqrt((x - target_x) * (x - target_x) + (y - target_y) * (y - target_y)));
}

static int	find_path(t_ctx *ctx, int target_x, int target_y)
{
	int	smallest = __INT_MAX__;
	int	dir = 0;
	int	pos_x;
	int	pos_y;

	pos_x = ctx->pos_x + 1;
	pos_y = ctx->pos_y;
	if (distance(pos_x, pos_y, target_x, target_y) < smallest
		&& is_walkable(ctx, pos_x, pos_y))
	{
		dir = 1;
		smallest = distance(pos_x, pos_y, target_x, target_y);
	}
	pos_x = ctx->pos_x;
	pos_y = ctx->pos_y + 1;
	if (distance(pos_x, pos_y, target_x, target_y) < smallest
		&& is_walkable(ctx, pos_x, pos_y))
	{
		dir = 2;
		smallest = distance(pos_x, pos_y, target_x, target_y);
	}
	pos_x = ctx->pos_x - 1;
	pos_y = ctx->pos_y;
	if (distance(pos_x, pos_y, target_x, target_y) < smallest
		&& is_walkable(ctx, pos_x, pos_y))
	{
		dir = 3;
		smallest = distance(pos_x, pos_y, target_x, target_y);
	}
	pos_x = ctx->pos_x;
	pos_y = ctx->pos_y - 1;
	if (distance(pos_x, pos_y, target_x, target_y) < smallest
		&& is_walkable(ctx, pos_x, pos_y))
	{
		dir = 4;
		smallest = distance(pos_x, pos_y, target_x, target_y);
	}
	return (dir);
}

void	move(t_ctx *ctx)
{
	if (ctx->pos_x != ctx->target_x || ctx->pos_y != ctx->target_y)
	{
		int dir = find_path(ctx, ctx->target_x, ctx->target_y);
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
