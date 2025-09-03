/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 23:22:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/03 12:00:45 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"
#include "ctx.h"
#include "game.h"

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

void    *error(const char *str)
{
    printf("Error: %s\n", str);
    return (NULL);
}

int running = 1;
void    handle_sigint(int sig)
{
    (void)sig;
    running = 0;
}

void	type_shit(t_ctx *ctx)
{
	if (!ctx->is_main_process)
		return ;
	
	t_message	message;

	message.mtype = 1;
	memset(message.text, 0, 64);

	msgsnd(ctx->message_id, &message, sizeof(t_message), IPC_NOWAIT);
}

int	wait_for_start(t_ctx *ctx)
{
    bool    started = false;
    sem_lock(ctx->semaphore_id);
    started = ctx->shared_memory->started;
    sem_unlock(ctx->semaphore_id);
    return (!started);
}

int	try_to_start(t_ctx *ctx)
{
    sem_lock(ctx->semaphore_id);
    
    render_board(ctx);

    t_gameinfo  gameinfos = get_game_infos(ctx);

    if (gameinfos.teams >= 2 && gameinfos.biggest_teams >= 2)
    {
        ctx->shared_memory->started = true;
        sem_unlock(ctx->semaphore_id);
        return (0);
    }
    sem_unlock(ctx->semaphore_id);
    return (1);
}

void    start_game(t_ctx *ctx)
{
    if (!ctx->is_main_process)
        while (wait_for_start(ctx) && running)
            usleep(10000);
    else
        while (try_to_start(ctx) && running)
            usleep(10000);
}

int main(int ac, char **av)
{
    t_ctx   ctx;

    if (!init_ctx(&ctx, ac, av))
        return (1);

    start_game(&ctx);

    printf("Starting\n");

    while (running)
    {
        if (!game_logic(&ctx))
            break ;
        usleep(10000);
    }

    return (delete_ctx(&ctx));
}
