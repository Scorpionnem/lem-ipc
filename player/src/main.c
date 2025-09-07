/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:28:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/07 20:01:33 by mbatty           ###   ########.fr       */
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

void	send_target_message(t_ctx *ctx)
{
	char	*msg;

	msg = ft_strjoin_free(ft_strdup("TARGET "), ft_itoa(ctx->target_x));
	msg = ft_strjoin_free(msg, ft_strdup(" "));	
	msg = ft_strjoin_free(msg, ft_itoa(ctx->target_y));
	send_message(msg, ctx->msgqid, ctx->team, players_in_team(ctx, ctx->team) - 1);
	free(msg);
}

void	free_2d(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	handle_exchanges(t_ctx *ctx)
{
	if (!ctx->leader)
	{
		char	*msg = receive_message(ctx->msgqid, ctx->team);
		if (msg)
		{
			char	**split_msg;
			if (!ft_strncmp(msg, "TARGET", ft_strlen("TARGET")))
			{
				split_msg = ft_split(msg, ' ');
				ctx->target_x = ft_atoi(split_msg[1]);
				ctx->target_y = ft_atoi(split_msg[2]);
				free_2d(split_msg);
				free(msg);
			}
			else if (!ft_strncmp(msg, "BECOME LEADER!", ft_strlen("BECOME LEADER!")))
				ctx->leader = true;
		}
	}	
	else
	{
		send_target_message(ctx);
	}
}

void	game_loop(t_ctx *ctx)
{
	if (ctx->shm->paused)
		return ;

	t_gameinfo	infos;

	infos = get_game_infos(ctx);
	if (is_dead(ctx) || infos.teams == 1 || infos.biggest_teams == 1)
	{
		g_running = false;
		if (ctx->leader)
			send_message("BECOME LEADER!", ctx->msgqid, ctx->team, 1);
		return ;
	}

	find_target(ctx);
	handle_exchanges(ctx);

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
	return (delete_ctx(&ctx));
}
