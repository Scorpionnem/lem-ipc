/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:41:09 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 12:10:16 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

int	get_players_in_team(t_ctx *ctx, int team)
{
	int	res = 0;
	for (int y = 0; y < BOARD_SIZE; y++)
		for (int x = 0; x < BOARD_SIZE; x++)
			if (ctx->shared_memory->board[y][x] == team)
				res++;
	return (res);
}

static int	replace_all(int board[BOARD_SIZE][BOARD_SIZE], int target)
{
	int	res;
	int	x;
	int	y;

	res = 0;
	y = -1;
	while (++y < BOARD_SIZE)
	{
		x = -1;
		while (++x < BOARD_SIZE)
		{
			if (board[y][x] == target)
			{
				board[y][x] = 0;
				res++;
			}
		}
	}
	return (res);
}

static void	count_game_infos(t_gameinfo *gameinfos,
	int board[BOARD_SIZE][BOARD_SIZE])
{
	int	x;
	int	y;
	int	players_in_team;

	y = -1;
	while (++y < BOARD_SIZE)
	{
		x = -1;
		while (++x < BOARD_SIZE)
		{
			if (board[y][x] != 0)
			{
				gameinfos->last_team_checked = board[y][x];
				players_in_team = replace_all(board, board[y][x]);
				if (players_in_team > gameinfos->biggest_teams)
				{
					gameinfos->biggest_teams = players_in_team;
					gameinfos->biggest_team = board[y][x];
				}
				gameinfos->teams++;
				gameinfos->players += players_in_team;
			}
		}
	}
}

t_gameinfo	get_game_infos(t_ctx *ctx)
{
	t_gameinfo	res;
	int			board[BOARD_SIZE][BOARD_SIZE];

	ft_memset(&res, 0, sizeof(t_gameinfo));
	ft_memcpy(&board, &ctx->shared_memory->board,
		sizeof(int [BOARD_SIZE][BOARD_SIZE]));
	count_game_infos(&res, board);
	return (res);
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
	{
		kill_cell(ctx);
		return (1);
	}
	return (0);
}

bool	is_in_bounds(t_ctx *ctx, int x, int y)
{
	int	point;

	point = 0;
	if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE)
	{
		point = ctx->shared_memory->board[y][x];
		if (point != ctx->team && point != 0)
			return (true);
	}
	return (false);
}
