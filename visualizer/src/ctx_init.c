/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:19:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 16:09:07 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

int	ctx_init_msgq(t_ctx *ctx);
int	ctx_init_sem(t_ctx *ctx);
int	ctx_init_shm(t_ctx *ctx);

int	ctx_init_game(t_ctx *ctx)
{
	sem_lock(ctx->semid);
	ctx->shm->counter++;
	ctx->team_selected = 1;
	sem_unlock(ctx->semid);
	return (1);
}

int	ctx_init_shared_variables(t_ctx *ctx)
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
