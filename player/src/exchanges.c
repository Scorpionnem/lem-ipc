/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exchanges.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:13:39 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 11:32:37 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static void	send_target_message(t_ctx *ctx)
{
	char	*msg;

	msg = ft_strjoin_free(ft_strdup("TARGET "), ft_itoa(ctx->target_x));
	msg = ft_strjoin_free(msg, ft_strdup(" "));
	msg = ft_strjoin_free(msg, ft_itoa(ctx->target_y));
	send_message(msg, ctx->msgqid,
		ctx->team, players_in_team(ctx, ctx->team) - 1);
	free(msg);
}

void	handle_exchanges(t_ctx *ctx)
{
	char	*msg;
	char	**split_msg;

	if (!ctx->leader)
	{
		msg = receive_message(ctx->msgqid, ctx->team);
		if (msg)
		{
			if (!ft_strncmp(msg, "TARGET", ft_strlen("TARGET")))
			{
				split_msg = ft_split(msg, ' ');
				if (!split_msg)
					return ;
				ctx->target_x = ft_atoi(split_msg[1]);
				ctx->target_y = ft_atoi(split_msg[2]);
				free_2d(split_msg);
			}
			else if (!ft_strncmp(msg, "BECOME LEADER!",
					ft_strlen("BECOME LEADER!")))
				ctx->leader = true;
			free(msg);
		}
	}
	else
		send_target_message(ctx);
}
