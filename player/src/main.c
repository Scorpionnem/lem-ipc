/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:28:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/09 11:28:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"
#include "ctx.h"

sig_atomic_t	g_running = 1;

void	handle_sigint(int sig)
{
	(void)sig;
	g_running = 0;
}

t_gameinfo	get_game_infos(t_ctx *ctx);
int		handle_exchanges(t_ctx *ctx);

void	game_loop(t_ctx *ctx)
{
	t_gameinfo	infos;

	if (ctx->shm->paused)
		return ;
	infos = get_game_infos(ctx);
	if (is_dead(ctx) || infos.teams == 1 || infos.biggest_teams == 1)
	{
		g_running = false;
		return ;
	}
	find_target(ctx);
	if (!handle_exchanges(ctx))
	{
		g_running = false;
		return ;
	}
	move(ctx);
}

int	main(int ac, char **av)
{
	t_ctx	ctx;

	signal(SIGINT, handle_sigint);
	if (!init_ctx(&ctx, ac, av))
		return (1);
	while (g_running)
	{
		sem_lock(ctx.semid);
		game_loop(&ctx);
		sem_unlock(ctx.semid);
		usleep(SLEEP_TIME);
	}
	if (ctx.leader)
		send_message("BECOME LEADER!", ctx.msgqid, ctx.team, 1);
	return (delete_ctx(&ctx));
}
