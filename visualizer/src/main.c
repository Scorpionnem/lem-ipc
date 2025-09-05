/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:28:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 10:56:41 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"
#include "ctx.h"

int	g_running = 1;

void	handle_sigint(int sig)
{
	(void)sig;
	g_running = 0;
}

#define RED "\033[31m"
#define RESET "\033[0m"
#define BLUE "\033[34m"

void	draw_board(t_ctx *ctx)
{
	int	y;
	int	x;

	y = -1;
	printf("\033c");
	while (++y < BOARD_SIZE)
	{
		x = -1;
		while (++x < BOARD_SIZE)
		{
			if (ctx->shm->board[y][x] != 0)
				printf(RED"%d"RESET, ctx->shm->board[y][x]);
			else
				printf("%d", ctx->shm->board[y][x]);
		}
		printf("\n");
	}
}

void	game_loop(t_ctx *ctx)
{
	sem_lock(ctx->semid);

	draw_board(ctx);
	printf("Connected users: %d\n", ctx->shm->counter);
	
	sem_unlock(ctx->semid);

	usleep(SLEEP_TIME);
}

int	main(void)
{
	t_ctx	ctx;

	signal(SIGINT, handle_sigint);
	if (!init_ctx(&ctx))
		return (1);
	while (g_running)
		game_loop(&ctx);
	return (delete_ctx(&ctx));
}
