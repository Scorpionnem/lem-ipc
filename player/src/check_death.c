/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_death.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 10:33:47 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 11:34:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static int	is_ennemy(t_ctx *ctx, int x, int y)
{
	int	point;

	point = 0;
	if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE)
	{
		point = ctx->shm->board[y][x];
		if (point != ctx->team && point > 0)
			return (ctx->shm->board[y][x]);
	}
	return (0);
}

static void	add_around(t_intpair teams_around[8], int team)
{
	int		i;

	i = 0;
	if (team == 0)
		return ;
	while (i < 8)
	{
		if (teams_around[i].first == team)
		{
			teams_around[i].second++;
			return ;
		}
		i++;
	}
	i = 0;
	while (i < 8)
	{
		if (teams_around[i].first == -1)
		{
			teams_around[i].first = team;
			teams_around[i].second = 1;
			return ;
		}
		i++;
	}
}

int	is_dead(t_ctx *ctx)
{
	int			i;
	t_intpair	teams_around[8];

	i = 0;
	ft_bzero(&teams_around, 8 * sizeof(t_intpair));
	while (i < 8)
		teams_around[i++].first = -1;
	add_around(teams_around, is_ennemy(ctx, ctx->pos_x + 1, ctx->pos_y + 1));
	add_around(teams_around, is_ennemy(ctx, ctx->pos_x - 1, ctx->pos_y - 1));
	add_around(teams_around, is_ennemy(ctx, ctx->pos_x + 1, ctx->pos_y - 1));
	add_around(teams_around, is_ennemy(ctx, ctx->pos_x - 1, ctx->pos_y + 1));
	add_around(teams_around, is_ennemy(ctx, ctx->pos_x + 1, ctx->pos_y));
	add_around(teams_around, is_ennemy(ctx, ctx->pos_x, ctx->pos_y + 1));
	add_around(teams_around, is_ennemy(ctx, ctx->pos_x - 1, ctx->pos_y));
	add_around(teams_around, is_ennemy(ctx, ctx->pos_x, ctx->pos_y - 1));
	i = 0;
	while (i < 8)
		if (teams_around[i++].second >= DEATH_THRESHOLD)
			return (1);
	return (0);
}
