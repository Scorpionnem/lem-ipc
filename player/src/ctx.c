/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:29:01 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 10:41:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static int	ctx_parse_args(t_ctx *ctx, int ac, char **av)
{
	if (ac != 2)
		return (error_int("Wrong amount of arguments"));
	ctx->team = ft_atoi(av[1]);
	if (ctx->team <= 0)
		return (error_int("Wrong team number (Should be > 0)"));
	return (1);
}

static int	ctx_init_shm(t_ctx *ctx)
{
	bool	first_access;

	first_access = false;
	ctx->shmid = shmget(ctx->key, sizeof(t_shared), IPC_CREAT | IPC_EXCL | 0666);
	if (ctx->shmid == -1)
	{
		ctx->shmid = shmget(ctx->key, sizeof(t_shared), 0);
		if (ctx->shmid == -1)
			return (error_int("Failed to access/create shared memory segment"));
	}
	else
		first_access = true;
	ctx->shm = shmat(ctx->shmid, NULL, 0);
	if (ctx->shm == (void *)-1)
		return (error_int("Failed to attach shared memory segment"));
	if (first_access)
	{
		ft_bzero(ctx->shm, sizeof(t_shared));
		ctx->shm->paused = true;
	}
	return (1);
}

static int	ctx_init_sem(t_ctx *ctx)
{
	t_semun	arg;

	ctx->semid = semget(ctx->key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if (ctx->semid == -1)
	{
		ctx->semid = semget(ctx->key, 1, 0);
		if (ctx->semid == -1)
			return (error_int("Failed to access/create semaphore"));
		return (1);
	}
	arg.val = 1;
	if (semctl(ctx->semid, 0, SETVAL, arg) == -1)
		return (error_int("Failed to execute control operation on semaphore"));
	return (1);
}

static int	ctx_init_msgq(t_ctx *ctx)
{
	ctx->msgqid = msgget(ctx->key, IPC_CREAT | IPC_EXCL | 0666);
	if (ctx->msgqid == -1)
	{
		ctx->msgqid = msgget(ctx->key, 0);
		if (ctx->msgqid == -1)
			return (error_int("Failed to access/create semaphore"));
	}
	return (1);
}

static int	ctx_init_shared_variables(t_ctx *ctx)
{
	ctx->key = ftok("Makefile", 42);
	if (ctx->key == -1)
		return (error_int("Failed to get IPC key"));
	if (!ctx_init_shm(ctx))
		return (0);
	if (!ctx_init_sem(ctx))
		return (0);
	if (!ctx_init_msgq(ctx))
		return (0);
	return (1);
}

int	ctx_init_game(t_ctx *ctx)
{
	sem_lock(ctx->semid);
	
	ctx->shm->counter++;
	srand(time(NULL) + ctx->shm->counter);

	if (!team_exists(ctx, ctx->team))
		ctx->leader = true;

	ctx->pos_x = rand() % BOARD_SIZE;
	ctx->pos_y = rand() % BOARD_SIZE;
	while (ctx->shm->board[ctx->pos_y][ctx->pos_x] != 0)
	{
		ctx->pos_x = rand() % BOARD_SIZE;
		ctx->pos_y = rand() % BOARD_SIZE;
	}
	ctx->shm->board[ctx->pos_y][ctx->pos_x] = ctx->team;

	sem_unlock(ctx->semid);
	return (1);
}

int	init_ctx(t_ctx *ctx, int ac, char **av)
{
	ft_bzero(ctx, sizeof(t_ctx));
	if (!ctx_parse_args(ctx, ac, av))
		return (0);
	if (!ctx_init_shared_variables(ctx))
		return (0);
	if (!ctx_init_game(ctx))
		return (delete_ctx(ctx));
	return (1);
}

int	delete_ctx(t_ctx *ctx)
{
	sem_lock(ctx->semid);
	ctx->shm->counter--;
	ctx->shm->board[ctx->pos_y][ctx->pos_x] = 0;
	if (ctx->shm->counter <= 0)
	{
		sem_unlock(ctx->semid);
		shmdt(ctx->shm);
		shmctl(ctx->shmid, IPC_RMID, NULL);
		semctl(ctx->semid, 0, IPC_RMID);
		msgctl(ctx->msgqid, IPC_RMID, NULL);
	}
	else
	{
		sem_unlock(ctx->semid);
		shmdt(ctx->shm);
	}
	return (EXIT_SUCCESS);
}
