/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 10:34:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 11:45:15 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"
#include <math.h>

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

bool	is_walkable(t_ctx *ctx, int x, int y)
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

int	distance(int x, int y, int target_x, int target_y)
{
	return (sqrt((x - target_x) * (x - target_x)
			+ (y - target_y) * (y - target_y)));
}

void	find_path2(t_ctx *ctx, int *dir, int *smallest);

static int	find_path(t_ctx *ctx, int target_x, int target_y)
{
	int	smallest;
	int	dir;

	find_path2(ctx, &dir, &smallest);
	if (distance(ctx->pos_x, ctx->pos_y + 1, target_x, target_y) < smallest
		&& is_walkable(ctx, ctx->pos_x, ctx->pos_y + 1))
	{
		dir = 2;
		smallest = distance(ctx->pos_x, ctx->pos_y + 1, target_x, target_y);
	}
	if (distance(ctx->pos_x - 1, ctx->pos_y, target_x, target_y) < smallest
		&& is_walkable(ctx, ctx->pos_x - 1, ctx->pos_y))
	{
		dir = 3;
		smallest = distance(ctx->pos_x - 1, ctx->pos_y, target_x, target_y);
	}
	if (distance(ctx->pos_x, ctx->pos_y - 1, target_x, target_y) < smallest
		&& is_walkable(ctx, ctx->pos_x, ctx->pos_y - 1))
	{
		dir = 4;
		smallest = distance(ctx->pos_x, ctx->pos_y - 1, target_x, target_y);
	}
	return (dir);
}

void	move(t_ctx *ctx)
{
	int	dir;

	if (ctx->pos_x != ctx->target_x || ctx->pos_y != ctx->target_y)
	{
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
}
