/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:28:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 18:42:24 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"
#include "ctx.h"
#include "game.h"

int	*running_instance(void);

void	handle_sigint(int sig)
{
	(void)sig;
	*running_instance() = 0;
}

int	wait_for_start(t_ctx *ctx)
{
	bool	started;

	started = false;
	sem_lock(ctx->semaphore_id);
	started = ctx->shared_memory->started;
	sem_unlock(ctx->semaphore_id);
	return (!started);
}

int	try_to_start(t_ctx *ctx)
{
	t_gameinfo	gameinfos;

	sem_lock(ctx->semaphore_id);
	render_board(ctx);
	gameinfos = get_game_infos(ctx);
	if (gameinfos.teams >= MIN_TEAMS_COUNT
		&& gameinfos.biggest_teams >= MIN_BIGGEST_TEAM_COUNT)
	{
		ctx->shared_memory->started = true;
		sem_unlock(ctx->semaphore_id);
		return (0);
	}
	sem_unlock(ctx->semaphore_id);
	return (1);
}

void	start_game(t_ctx *ctx)
{
	if (!ctx->is_main_process)
		while (wait_for_start(ctx) && *running_instance())
			usleep(SLEEP_TIME);
	else
		while (try_to_start(ctx) && *running_instance())
			usleep(SLEEP_TIME);
}

int	is_game_running(t_ctx *ctx);

int	main(int ac, char **av)
{
	t_ctx	ctx;

	if (ac != 2)
		return (!error("Error: wrong amount of arguments"));
	srand(time(NULL));
	if (!init_ctx(&ctx, av))
		return (1);
	start_game(&ctx);
	while (*running_instance())
	{
		sem_lock(ctx.semaphore_id);
		if (!game_logic(&ctx))
		{
			sem_unlock(ctx.semaphore_id);
			break ;
		}
		sem_unlock(ctx.semaphore_id);
		usleep(SLEEP_TIME);
	}
	while (ctx.is_main_process && *running_instance())
	{
		sem_lock(ctx.semaphore_id);
		render_board(&ctx);
		if (!is_game_running(&ctx))
		{
			sem_unlock(ctx.semaphore_id);
			break ;
		}
		sem_unlock(ctx.semaphore_id);
		usleep(SLEEP_TIME / 10);
	}
	while (ctx.is_main_process)
	{
		sem_lock(ctx.semaphore_id);
		if (ctx.shared_memory->counter == 1)
		{
			sem_unlock(ctx.semaphore_id);
			break ;
		}
		sem_unlock(ctx.semaphore_id);
	}
	return (delete_ctx(&ctx));
}
