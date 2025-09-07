/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:28:13 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/07 11:47:06 by mbatty           ###   ########.fr       */
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

typedef struct s_gameinfo
{
	int	teams;
	int	last_team_checked;
	int	biggest_teams;
	int	biggest_team;
	int	players;
}	t_gameinfo;

typedef struct s_intpair
{
	int	first;
	int	second;
}	t_intpair;

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
	bool	paused;
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
	int			prev_pos_x;
	int			prev_pos_y;
	int			team;
	bool		leader;
	int			target_x;
	int			target_y;

	key_t		key;
	//Shared memory
	t_shared	*shm;
}	t_ctx;

int		init_ctx(t_ctx *ctx, int ac, char **av);
int		delete_ctx(t_ctx *ctx);

int		is_dead(t_ctx *ctx);
void	move(t_ctx *ctx);
bool	team_exists(t_ctx *ctx, int team);
void	find_target(t_ctx *ctx);
int		distance(int x, int y, int target_x, int target_y);
int		players_in_team(t_ctx *ctx, int team);

#endif
