/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:29:01 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/03 14:40:02 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static int get_shared_memory(t_ctx *ctx)
{
    ctx->is_main_process = false;

    ctx->shared_memory_id = shmget(ctx->ipckey, sizeof(shared_t), IPC_CREAT | 0666);
    if (ctx->shared_memory_id == -1)
        return (!!error("Error getting shared memory id"));

    ctx->shared_memory = shmat(ctx->shared_memory_id, NULL, 0);
    if (ctx->shared_memory == (void *) -1)
        return (!!error("Error attaching shared memory"));

    struct shmid_ds buf;
    shmctl(ctx->shared_memory_id, IPC_STAT, &buf);
    if (buf.shm_nattch == 1)
        ctx->is_main_process = true;

    return (1);
}

static int get_semaphore(t_ctx *ctx)
{
    ctx->semaphore_id = semget(ctx->ipckey, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (ctx->semaphore_id != -1)
    {
        union semun
        {
            int val;
            struct semid_ds *buf;
            unsigned short *array;
        } arg;

        arg.val = 1; // start unlocked
        if (semctl(ctx->semaphore_id, 0, SETVAL, arg) == -1)
			return (0);
    }
    else // Semaphore already inited
    {
        ctx->semaphore_id = semget(ctx->ipckey, 1, 0666);
        if (ctx->semaphore_id == -1)
        {
			ft_putendl_fd("Error: could not get existing semaphore", 2);
            return 0;
        }
    }

    return 1;
}

void    handle_sigint(int sig);

int init_ctx(t_ctx *ctx, int ac, char **av)
{
    signal(SIGINT, handle_sigint);

	if (ac != 2)
	{
		ft_putendl_fd("Error: wrong amount of arguments", 2);
		return (0);
	}

	ctx->team = atoi(av[1]);
	if (ctx->team <= 0)
	{
		ft_putendl_fd("Error: team can only be a positive number", 2);
		return (0);
	}
	
    ctx->ipckey = ftok("src/ctx.c", 42);
	if (ctx->ipckey == -1)
		return (0);

    if (!get_semaphore(ctx))
        return (0);

    sem_lock(ctx->semaphore_id);

    if (!get_shared_memory(ctx))
        return (0);

    if (ctx->is_main_process)
    {
        for (int y = 0; y < BOARD_SIZE; y++)
            for (int x = 0; x < BOARD_SIZE; x++)
                ctx->shared_memory->board[y][x] = 0;
        ctx->shared_memory->counter = 0;
        ctx->shared_memory->started = false;
    }

    ctx->pos_x = ctx->shared_memory->counter;
    ctx->pos_y = ctx->shared_memory->counter;
	ctx->is_dead = false;
	ctx->game_done = false;

    ctx->shared_memory->board[ctx->pos_y][ctx->pos_x] = ctx->team;

    ctx->shared_memory->counter++;
    sem_unlock(ctx->semaphore_id);

    return (1);
}

int delete_ctx(t_ctx *ctx)
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
    }
    else
        shmdt(ctx->shared_memory);

    return (EXIT_SUCCESS);
}
