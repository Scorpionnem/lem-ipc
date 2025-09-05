/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:28:13 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 12:15:09 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CTX_H
# define CTX_H

# include "libs.h"

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
	int		counter;
	bool	started;
}	t_shared;

typedef struct s_ctx
{
	int			shared_memory_id;
	int			semaphore_id;
	int			message_id;

	int			pos_x;
	int			pos_y;
	int			team;

	int			target_x;
	int			target_y;

	bool		is_dead;
	bool		is_main_process;
	bool		game_done;
	bool		is_team_leader;

	key_t		ipckey;
	t_shared	*shared_memory;
}	t_ctx;

int		init_ctx(t_ctx *ctx, char **av);
int		delete_ctx(t_ctx *ctx);

void	*error(char *str);
void	sem_lock(int semid);
void	sem_unlock(int semid);
void	kill_cell(t_ctx *ctx);

#endif
