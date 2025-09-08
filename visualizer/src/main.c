/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:28:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 11:08:23 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"
#include "ctx.h"

int	g_running = 1;

int			put_strings(t_ctx *ctx);

void	handle_sigint(int sig)
{
	(void)sig;
	g_running = 0;
}

int	close_window(t_ctx *ctx)
{
	mlx_loop_end(ctx->mlx);
	return (0);
}

#define ESC_KEY 65307
#define SPACE_KEY 32

int	key_hook(int keycode, t_ctx *ctx)
{
	if (keycode == ESC_KEY)
		close_window(ctx);
	if (keycode == SPACE_KEY)
	{
		sem_lock(ctx->semid);
		ctx->shm->paused = !ctx->shm->paused;
		sem_unlock(ctx->semid);
	}
	return (0);
}

int	loop_hook(t_ctx *ctx)
{
	sem_lock(ctx->semid);

	draw_board(ctx);
	mlx_put_image_to_window(ctx->mlx, ctx->mlx_win, ctx->img.data, 0, 0);
	if (!put_strings(ctx) || !g_running)
	{
		sem_unlock(ctx->semid);
		return (close_window(ctx));
	}

	sem_unlock(ctx->semid);
	return (1);
}

void	handle_click(int button, int x, int y, t_ctx *ctx)
{
(void)button;
	sem_lock(ctx->semid);
	x = (float)x / WIN_SIZE * BOARD_SIZE;
	y = (float)y / WIN_SIZE * BOARD_SIZE;
	if (button == MOUSE_LEFT_CLICK && ctx->shm->board[y][x] == -1)
		ctx->shm->board[y][x] = 0;
	else if (button == MOUSE_RIGHT_CLICK && ctx->shm->board[y][x] == 0)
		ctx->shm->board[y][x] = -1;
	sem_unlock(ctx->semid);
}

int mouse_press(int button, int x, int y, t_ctx *ctx)
{
	(void)ctx; // unused
	if (button == MOUSE_LEFT_CLICK || button == MOUSE_RIGHT_CLICK)
		handle_click(button, x, y, ctx);
	return (0);
}

void	set_hooks(t_ctx *ctx)
{
	mlx_hook(ctx->mlx_win, 17, 0, close_window, ctx);
	mlx_loop_hook(ctx->mlx, loop_hook, ctx);
	mlx_key_hook(ctx->mlx_win, key_hook, ctx);
	mlx_mouse_hook(ctx->mlx_win, mouse_press, ctx);
	mlx_loop(ctx->mlx);
}

void	free_mlx(t_ctx *ctx)
{
	mlx_destroy_image(ctx->mlx, ctx->img.data);
	mlx_destroy_window(ctx->mlx, ctx->mlx_win);
	mlx_destroy_display(ctx->mlx);
	free(ctx->mlx);
}

void	init_mlx(t_ctx *ctx)
{
	ctx->mlx = mlx_init();
	if (!ctx->mlx)
		return ;
	ctx->mlx_win = mlx_new_window(ctx->mlx, WIN_SIZE, WIN_SIZE, "lem-ipc-visualizer");
	if (!ctx->mlx_win)
		return ;
	ctx->img.data = mlx_new_image(ctx->mlx, WIN_SIZE, WIN_SIZE);
	if (!ctx->img.data)
		return ;
	ctx->img.addr = mlx_get_data_addr(ctx->img.data, &ctx->img.bits_per_pixel, &ctx->img.line_length,
								&ctx->img.endian);
	set_hooks(ctx);
	free_mlx(ctx);
}

int	main(void)
{
	t_ctx	ctx;

	signal(SIGINT, handle_sigint);
	if (!init_ctx(&ctx))
		return (1);
	init_mlx(&ctx);
	return (delete_ctx(&ctx));
}
