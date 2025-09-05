/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:29:01 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 18:42:41 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static int	get_shared_memory(t_ctx *ctx)
{
	struct shmid_ds	buf;

	ctx->is_main_process = false;
	ctx->shared_memory_id = shmget(ctx->ipckey,
			sizeof(t_shared), IPC_CREAT | 0666);
	if (ctx->shared_memory_id == -1)
		return (!!error("Error getting shared memory id"));
	ctx->shared_memory = shmat(ctx->shared_memory_id, NULL, 0);
	if (ctx->shared_memory == (void *) -1)
		return (!!error("Error attaching shared memory"));
	shmctl(ctx->shared_memory_id, IPC_STAT, &buf);
	if (buf.shm_nattch == 1)
		ctx->is_main_process = true;
	return (1);
}

static int	get_semaphore(t_ctx *ctx)
{
	t_semun	arg;

	ctx->semaphore_id = semget(ctx->ipckey, 1, IPC_CREAT | IPC_EXCL | 0666);
	if (ctx->semaphore_id != -1)
	{
		arg.val = 1;
		if (semctl(ctx->semaphore_id, 0, SETVAL, arg) == -1)
			return (0);
	}
	else
	{
		ctx->semaphore_id = semget(ctx->ipckey, 1, 0666);
		if (ctx->semaphore_id == -1)
		{
			ft_putendl_fd("Error: could not get existing semaphore", 2);
			return (0);
		}
	}
	return (1);
}

void	handle_sigint(int sig);

int	get_players_in_team(t_ctx *ctx, int team);

void	spawn_player(t_ctx *ctx)
{
	int	target_x;
	int	target_y;

	target_x = rand() % BOARD_SIZE;
	target_y = rand() % BOARD_SIZE;
	ctx->is_team_leader = get_players_in_team(ctx, ctx->team) == 0;
	while (ctx->shared_memory->board[target_y][target_x] != 0)
	{
		target_x = rand() % BOARD_SIZE;
		target_y = rand() % BOARD_SIZE;
	}
	ctx->pos_x = target_x;
	ctx->pos_y = target_y;
	ctx->shared_memory->board[ctx->pos_y][ctx->pos_x] = ctx->team;
}

static int	get_messages(t_ctx *ctx)
{
	ctx->message_id = msgget(ctx->ipckey, IPC_CREAT | IPC_EXCL | 0666);
	if (ctx->message_id == -1)
	{
		ctx->message_id = msgget(ctx->ipckey, 0);
	}
	return (1);
}

int	init_ctx(t_ctx *ctx, char **av)
{
	ft_bzero(ctx, sizeof(t_ctx));
	signal(SIGINT, handle_sigint);
	ctx->team = atoi(av[1]);
	if (ctx->team <= 0)
		return (!!error("Error: team can only be a positive number (> 0)"));
	ctx->ipckey = ftok("src/ctx.c", 42);
	if (ctx->ipckey == -1)
		return (!!error("Error: failed to get ftoken"));
	if (!get_messages(ctx))
		return (0);
	if (!get_semaphore(ctx))
		return (0);
	sem_lock(ctx->semaphore_id);
	if (!get_shared_memory(ctx))
		return (0);
	if (ctx->is_main_process)
	{
		ft_memset(&ctx->shared_memory->board, 0,
			sizeof(int [BOARD_SIZE][BOARD_SIZE]));
		ctx->shared_memory->counter = 0;
		ctx->shared_memory->started = false;
	}
	spawn_player(ctx);
	ctx->is_dead = false;
	ctx->game_done = false;
	ctx->shared_memory->counter++;
	sem_unlock(ctx->semaphore_id);
	return (1);
}

int	delete_ctx(t_ctx *ctx)
{
	sem_lock(ctx->semaphore_id);
	kill_cell(ctx);
	ctx->shared_memory->counter--;
	sem_unlock(ctx->semaphore_id);
	if (ctx->shared_memory->counter <= 0)
	{
		ft_putendl_fd("Last process to leave, cleaning up.", 1);
		shmdt(ctx->shared_memory);
		shmctl(ctx->shared_memory_id, IPC_RMID, NULL);
		semctl(ctx->semaphore_id, 0, IPC_RMID);
		msgctl(ctx->message_id, IPC_RMID, NULL);
	}
	else
		shmdt(ctx->shared_memory);
	return (EXIT_SUCCESS);
}
