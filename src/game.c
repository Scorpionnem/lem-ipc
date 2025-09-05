/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:37:56 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 18:29:35 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	                   send_message(t_ctx	*ctx, char *str);

void	kill_cell(t_ctx *ctx)
{
	ctx->shared_memory->board[ctx->pos_y][ctx->pos_x] = 0;
	if (ctx->is_dead || ctx->game_done)
		return ;
	if (ctx->is_team_leader)
		send_message(ctx, "REPLACE");
	ctx->is_dead = true;
}

int	is_game_running(t_ctx *ctx)
{
	t_gameinfo	infos;

	infos = get_game_infos(ctx);
	if (infos.teams == 1)
	{
		if (ctx->is_main_process)
		{
			ft_putstr_fd("Team ", 1);
			ft_putnbr_fd(infos.last_team_checked, 1);
			ft_putendl_fd(" wins!", 1);
		}
		ctx->game_done = true;
		return (0);
	}
	if (infos.biggest_teams == 1)
	{
		ft_putendl_fd("Its a tie!", 1);
		ctx->game_done = true;
		return (0);
	}
	return (1);
}

static void	free_2d(char **str)
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

void	receive_message(t_ctx *ctx)
{
	t_message	msg = {0};
	char		**input;
	
	if (msgrcv(ctx->message_id, &msg, sizeof(msg.mtext), ctx->team, IPC_NOWAIT) == -1)
		return ;

	if (ft_strncmp(msg.mtext, "ATTACK", ft_strlen("ATTACK")) == 0)
	{
		input = ft_split(msg.mtext, ' ');
		ctx->target_x = ft_atoi(input[1]);
		ctx->target_y = ft_atoi(input[2]);
		free_2d(input);
	}
	else if (ft_strncmp(msg.mtext, "REPLACE", ft_strlen("REPLACE")) == 0)
		ctx->is_team_leader = true;
}

void	send_message(t_ctx *ctx, char *str)
{
	t_message	msg = {0};

	msg.mtype = ctx->team;
	ft_strlcpy(msg.mtext, str, ft_strlen(str) + 1);
	for (int i = 0; i < get_players_in_team(ctx, ctx->team) - 1; i++)
		msgsnd(ctx->message_id, &msg, sizeof(msg.mtext), 0);
}

void	handle_exchanges(t_ctx *ctx)
{
	char	*str;
	
	if (ctx->is_team_leader)
	{
		str = ft_strjoin_free(ft_strdup("ATTACK "), ft_itoa(ctx->target_x));
		str = ft_strjoin_free(str, ft_strdup(" "));
		str = ft_strjoin_free(str, ft_itoa(ctx->target_y));
		send_message(ctx, str);
		free(str);
	}
	else
		receive_message(ctx);
}

int	game_logic(t_ctx *ctx)
{
	if (!is_game_running(ctx))
		return (0);
	is_dead(ctx);
	if (ctx->is_dead)
		return (0);
	handle_exchanges(ctx);
	move(ctx);
	if (ctx->is_main_process)
		render_board(ctx);
	return (1);
}
