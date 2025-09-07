/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 10:37:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/07 10:53:42 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

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
