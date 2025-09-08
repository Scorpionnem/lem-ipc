/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 10:37:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 11:45:26 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

bool	is_walkable(t_ctx *ctx, int x, int y);
int		distance(int x, int y, int target_x, int target_y);

void	find_path2(t_ctx *ctx, int *dir, int *smallest)
{
	*dir = 0;
	*smallest = __INT_MAX__;
	if (distance(ctx->pos_x + 1, ctx->pos_y,
			ctx->target_x, ctx->target_y) < *smallest
		&& is_walkable(ctx, ctx->pos_x + 1, ctx->pos_y))
	{
		*dir = 1;
		*smallest = distance(ctx->pos_x + 1, ctx->pos_y,
				ctx->target_x, ctx->target_y);
	}
}

void	find_target(t_ctx *ctx)
{
	int			min_distance;
	int			y;
	int			x;

	if (!ctx->leader)
		return ;
	min_distance = __INT_MAX__;
	y = -1;
	while (++y < BOARD_SIZE)
	{
		x = -1;
		while (++x < BOARD_SIZE)
		{
			if (ctx->shm->board[y][x] > 0
				&& ctx->shm->board[y][x] != ctx->team
				&& distance(ctx->pos_x, ctx->pos_y, x, y) < min_distance)
			{
				ctx->target_x = x;
				ctx->target_y = y;
				min_distance = distance(ctx->pos_x, ctx->pos_y, x, y);
			}
		}
	}
}
