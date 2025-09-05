/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 10:37:08 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 10:43:46 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void	receive_message(t_ctx *ctx)
// {
// 	t_message	msg = {0};
// 	char		**input;
	
// 	if (msgrcv(ctx->message_id, &msg, sizeof(msg.mtext), ctx->team, IPC_NOWAIT) == -1)
// 		return ;

// 	if (ft_strncmp(msg.mtext, "ATTACK", ft_strlen("ATTACK")) == 0)
// 	{
// 		input = ft_split(msg.mtext, ' ');
// 		ctx->target_x = ft_atoi(input[1]);
// 		ctx->target_y = ft_atoi(input[2]);
// 		free_2d(input);
// 	}
// 	else if (ft_strncmp(msg.mtext, "REPLACE", ft_strlen("REPLACE")) == 0)
// 		ctx->is_team_leader = true;
// }

// void	send_message(t_ctx *ctx, char *str)
// {
// 	t_message	msg = {0};

// 	msg.mtype = ctx->team;
// 	ft_strlcpy(msg.mtext, str, ft_strlen(str) + 1);
// 	for (int i = 0; i < get_players_in_team(ctx, ctx->team) - 1; i++)
// 		msgsnd(ctx->message_id, &msg, sizeof(msg.mtext), 0);
// }

#include "ctx.h"

char	*receive_message(int msgqid, int channel)
{
	t_message	msg;
	
	ft_bzero(&msg, sizeof(t_message));
	if (msgrcv(msgqid, &msg, sizeof(msg.mtext), channel, IPC_NOWAIT) == -1)
		return (NULL);
	return (ft_strdup(msg.mtext));
}

int	send_message(char *msg, int msgqid, int channel, int targets_count)
{
	t_message	msgdata;
	int			i;

	i = 0;
	ft_bzero(&msgdata, sizeof(t_message));
	msgdata.mtype = channel;
	ft_strlcpy(msgdata.mtext, msg, ft_strlen(msg) + 1);
	while (i < targets_count)
	{
		if (msgsnd(msgqid, &msgdata, sizeof(msgdata.mtext), 0) == -1)
			return (error_int("Failed to send message"));
		i++;
	}
	return (1);
}
