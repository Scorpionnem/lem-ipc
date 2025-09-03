/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:41:09 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/03 11:48:25 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static int    replace_all(int board[BOARD_SIZE][BOARD_SIZE], int target)
{
    int res = 0;
    
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
            if (board[y][x] == target)
            {
                board[y][x] = 0;
                res++;
            }

    return (res);
}

t_gameinfo  get_game_infos(t_ctx *ctx)
{
    t_gameinfo res = {0};
    int board[BOARD_SIZE][BOARD_SIZE];

    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
            board[y][x] = ctx->shared_memory->board[y][x];

    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (board[y][x] != 0)
            {
				res.last_team_checked = board[y][x];
                int players_in_team = replace_all(board, board[y][x]);
                if (players_in_team > res.biggest_teams)
				{
                    res.biggest_teams = players_in_team;
					res.biggest_team = board[y][x];
				}
                res.teams++;
                res.players += players_in_team;
            }
        }

    return (res);
}

#define DEATH_THRESHOLD 2
int is_dead(t_ctx *ctx)
{
    int neighbours = 0;

    neighbours += is_in_bounds(ctx, ctx->pos_x + 1, ctx->pos_x + 1);
    neighbours += is_in_bounds(ctx, ctx->pos_x - 1, ctx->pos_x - 1);
    neighbours += is_in_bounds(ctx, ctx->pos_x + 1, ctx->pos_x - 1);
    neighbours += is_in_bounds(ctx, ctx->pos_x - 1, ctx->pos_x + 1);
    neighbours += is_in_bounds(ctx, ctx->pos_x + 1, ctx->pos_x);
    neighbours += is_in_bounds(ctx, ctx->pos_x, ctx->pos_x + 1);
    neighbours += is_in_bounds(ctx, ctx->pos_x - 1, ctx->pos_x);
    neighbours += is_in_bounds(ctx, ctx->pos_x, ctx->pos_x - 1);
    
    printf("I have %d enemies around\n", neighbours);

    if (neighbours >= DEATH_THRESHOLD)
    {
        kill_cell(ctx);
        return (1);
    }

    return (0);
}

bool    is_in_bounds(t_ctx *ctx, int x, int y)
{
    int point = 0;

    if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE)
    {
        point = ctx->shared_memory->board[y][x];
        if (point != ctx->team && point != 0)
            return (true);
    }
    return (false);
}
