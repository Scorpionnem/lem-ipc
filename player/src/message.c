/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 10:37:08 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/07 20:16:27 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (msgsnd(msgqid, &msgdata, sizeof(msgdata.mtext), IPC_NOWAIT) == -1)
			return (0);
		i++;
	}
	return (1);
}
