/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 23:22:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/03 00:05:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"

typedef struct
{
    long int    mtype;
    char        text[64];
} message_t;

void sem_lock(int semid)
{
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_unlock(int semid)
{
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

#define BOARD_SIZE 20

typedef struct
{
    int board[BOARD_SIZE][BOARD_SIZE];
    int counter;
} shared_t;

typedef struct
{
    int         shared_memory_id;
    int         semaphore_id;
    int         message_id;

    int         pos_x;
    int         pos_y;
    int         team;

    bool        is_main_process;

    key_t       ipckey;
    shared_t    *shared_memory;
} t_ctx;

void    *error(const char *str)
{
    printf("Error: %s\n", str);
    return (NULL);
}

int get_shared_memory(t_ctx *ctx)
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

int get_semaphore(t_ctx *ctx)
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
        semctl(ctx->semaphore_id, 0, SETVAL, arg);
    }
    else // Semaphore already inited
    {
        ctx->semaphore_id = semget(ctx->ipckey, 1, 0666);
        if (ctx->semaphore_id == -1)
        {
            printf("Error: could not get existing semaphore\n");
            return 0;
        }
    }

    return 1;
}

int init_ctx(t_ctx *ctx)
{
    ctx->ipckey = ftok("ipc_demo.c", 42);

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
    }

    ctx->pos_x = ctx->shared_memory->counter;
    ctx->pos_y = ctx->shared_memory->counter;
    ctx->team = 1;

    ctx->shared_memory->board[ctx->pos_y][ctx->pos_x] = ctx->team;

    ctx->shared_memory->counter++;
    sem_unlock(ctx->semaphore_id);

    ctx->message_id = msgget(ctx->ipckey, IPC_CREAT | 0666);

    return (1);
}

int delete_ctx(t_ctx *ctx)
{
    sem_lock(ctx->semaphore_id);

    ctx->shared_memory->counter--;
    ctx->shared_memory->board[ctx->pos_y][ctx->pos_x] = 0;

    sem_unlock(ctx->semaphore_id);

    if (ctx->shared_memory->counter <= 0)
    {
        printf("Last to leave\n");
        shmdt(ctx->shared_memory);
        shmctl(ctx->shared_memory_id, IPC_RMID, NULL);
        msgctl(ctx->message_id, IPC_RMID, NULL);
        semctl(ctx->semaphore_id, 0, IPC_RMID);
    }
    else
        shmdt(ctx->shared_memory);

    return (EXIT_SUCCESS);
}

#include <signal.h>

int running = 0;
void    handle_sigint(int)
{
    running = 1;
}

void    render_board(t_ctx *ctx)
{
    printf("\033cGame Board:\n");
    sem_lock(ctx->semaphore_id);
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
            printf("%d", ctx->shared_memory->board[y][x]);
        printf("\n");
    }
    printf("There are %d players\n", ctx->shared_memory->counter);
    sem_unlock(ctx->semaphore_id);
}

void    move_to(t_ctx *ctx, int target_x, int target_y)
{
    if (target_x < 0 || target_x >= BOARD_SIZE || target_y < 0 || target_y >= BOARD_SIZE)
        return ;

    if (ctx->shared_memory->board[target_y][target_x] == 0)
    {
        ctx->shared_memory->board[target_y][target_x] = ctx->team;
        ctx->shared_memory->board[ctx->pos_y][ctx->pos_x] = 0;
        ctx->pos_x = target_x;
        ctx->pos_y = target_y;
    }
}

int    game_logic(t_ctx *ctx)
{
    if (ctx->is_main_process)
        render_board(ctx);

    sem_lock(ctx->semaphore_id);

    move_to(ctx, ctx->pos_x + 1, ctx->pos_y + 1);

    sem_unlock(ctx->semaphore_id);

    sleep(1);
    return (1);
}

int main()
{
    t_ctx   ctx;

    if (!init_ctx(&ctx))
        return (1);

    signal(SIGINT, handle_sigint);

    while (!running)
        if (!game_logic(&ctx))
            break ;

    return (delete_ctx(&ctx));
}
