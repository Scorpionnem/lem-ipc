/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_infos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 22:33:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/06 22:36:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

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
	ft_memcpy(&board, &ctx->shm->board,
		sizeof(int [BOARD_SIZE][BOARD_SIZE]));
	count_game_infos(&res, board);
	return (res);
}

