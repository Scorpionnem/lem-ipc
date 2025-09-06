/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:28:13 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 10:12:55 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CTX_H
# define CTX_H

# include "libs.h"
# include "error.h"

# define BOARD_SIZE 20

# define MIN_TEAMS_COUNT 2
# define MIN_BIGGEST_TEAM_COUNT 2
# define DEATH_THRESHOLD 2
# define SLEEP_TIME 200000

typedef union u_semun
{
	int				val;
	struct semid_ds	*buf;
	unsigned short	*array;
}	t_semun;

typedef struct s_message
{
	long	mtype;
	char	mtext[64];
}	t_message;

typedef struct s_shared
{
	int		board[BOARD_SIZE][BOARD_SIZE];
	//Counts processes that have access to the shared memory
	int		counter;
	bool	started;
}	t_shared;

typedef struct s_ctx
{
	//Shared memory id
	int			shmid;
	//Semaphore id
	int			semid;
	//Message queue id
	int			msgqid;

	int			pos_x;
	int			pos_y;
	int			team;

	key_t		key;
	//Shared memory
	t_shared	*shm;
}	t_ctx;

int		init_ctx(t_ctx *ctx, int ac, char **av);
int		delete_ctx(t_ctx *ctx);

#endif
