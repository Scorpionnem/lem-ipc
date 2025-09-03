/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:37:56 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/03 12:10:29 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void    move_to(t_ctx *ctx, int target_x, int target_y)
{
    if (target_x < 0 || target_x >= BOARD_SIZE || target_y < 0 || target_y >= BOARD_SIZE)
        return ;

    if (ctx->shared_memory->board[target_y][target_x] == 0)
    {
        ctx->shared_memory->board[target_y][target_x] = ctx->team;
        ctx->shared_memory->board[ctx->pos_y][ctx->pos_x] = 0;
        ctx->pos_x = target_x;
        ctx->pos_y = target_y;
    }
}

static void    move(t_ctx *ctx)
{
    int move_x = (rand() % 2) * rand() % 2 == 0 ? 1 : -1;
    int move_y = (rand() % 2) * rand() % 2 == 0 ? 1 : -1;

    if (rand() % 2)
        move_to(ctx, ctx->pos_x + move_x, ctx->pos_y);
    else
        move_to(ctx, ctx->pos_x, ctx->pos_y + move_y);
}

void    kill_cell(t_ctx *ctx)
{
    if (ctx->is_dead || ctx->game_done)
        return ;
    printf("I died...\n");
    ctx->is_dead = true;
    ctx->shared_memory->board[ctx->pos_y][ctx->pos_x] = 0;
}

int	is_game_running(t_ctx *ctx)
{
	t_gameinfo infos = get_game_infos(ctx);
	
	if (infos.teams == 1)
	{
		printf("Team %d wins!\n", infos.last_team_checked);
		ctx->game_done = true;
		return (0);
	}
	return (1);
}

int    game_logic(t_ctx *ctx)
{
    sem_lock(ctx->semaphore_id);

	if (ctx->is_main_process && !ctx->is_dead)
		render_board(ctx);

	if (!is_game_running(ctx))
	{
    	sem_unlock(ctx->semaphore_id);
		return (0);
	}

	is_dead(ctx);
    if (ctx->is_dead)
    {
        sem_unlock(ctx->semaphore_id);
        return (ctx->is_main_process);
    }

    move(ctx);

	if (ctx->is_main_process && !ctx->is_dead)
		render_board(ctx);

    sem_unlock(ctx->semaphore_id);

    return (1);
}
