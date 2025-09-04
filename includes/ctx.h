/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:28:13 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/03 14:59:13 by mbatty           ###   ########.fr       */
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

typedef struct
{
	int		board[BOARD_SIZE][BOARD_SIZE];
	int		counter;
	bool	started;
} shared_t;

typedef struct
{
	int         shared_memory_id;
	int         semaphore_id;

	int         pos_x;
	int         pos_y;
	int         team;

	bool		is_dead;
	bool        is_main_process;
	bool		game_done;

	key_t       ipckey;
	shared_t    *shared_memory;
} t_ctx;

int init_ctx(t_ctx *ctx, int ac, char **av);
int delete_ctx(t_ctx *ctx);

void    *error(char *str);
void 	sem_lock(int semid);
void 	sem_unlock(int semid);
void    kill_cell(t_ctx *ctx);

#endif
