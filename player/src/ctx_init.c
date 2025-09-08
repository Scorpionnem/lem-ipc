/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:46:30 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 12:16:39 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

int	ctx_parse_args(t_ctx *ctx, int ac, char **av);
int	ctx_init_shm(t_ctx *ctx);
int	ctx_init_sem(t_ctx *ctx);
int	ctx_init_msgq(t_ctx *ctx);

static int	ctx_init_shared_variables(t_ctx *ctx)
{
	ctx->key = ftok("Makefile", 42);
	if (ctx->key == -1)
		return (error_int("Failed to get IPC key"));
	if (!ctx_init_shm(ctx))
		return (0);
	if (!ctx_init_sem(ctx))
		return (0);
	if (!ctx_init_msgq(ctx))
		return (0);
	return (1);
}

int	ctx_init_game(t_ctx *ctx)
{
	sem_lock(ctx->semid);
	ctx->shm->counter++;
	srand(time(NULL) + ctx->shm->counter);
	if (!team_exists(ctx, ctx->team))
		ctx->leader = true;
	ctx->pos_x = rand() % BOARD_SIZE;
	ctx->pos_y = rand() % BOARD_SIZE;
	if (!players_in_team(ctx, 0))
		return (error_int("No space left on map"));
	while (ctx->shm->board[ctx->pos_y][ctx->pos_x] != 0)
	{
		ctx->pos_x = rand() % BOARD_SIZE;
		ctx->pos_y = rand() % BOARD_SIZE;
	}
	ctx->shm->board[ctx->pos_y][ctx->pos_x] = ctx->team;
	sem_unlock(ctx->semid);
	return (1);
}

int	init_ctx(t_ctx *ctx, int ac, char **av)
{
	ft_bzero(ctx, sizeof(t_ctx));
	if (!ctx_parse_args(ctx, ac, av))
		return (0);
	if (!ctx_init_shared_variables(ctx))
		return (0);
	if (!ctx_init_game(ctx))
		return (delete_ctx(ctx));
	return (1);
}
