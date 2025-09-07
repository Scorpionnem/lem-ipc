/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:28:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/07 20:11:26 by mbatty           ###   ########.fr       */
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

void	put_pixel(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
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

int	darken(int color, float factor)
{
	int r = ((color >> 16) & 0xFF) * factor;
	int g = ((color >> 8) & 0xFF) * factor;
	int b = (color & 0xFF) * factor;
	return ((r << 16) | (g << 8) | b);
}

#define SQUARE_SIZE 32
void	put_square(t_ctx *ctx, int x, int y, int color)
{
	int	cur_x;
	int	cur_y;

	cur_x = 0;
	while (cur_x < SQUARE_SIZE)
	{
		cur_y = 0;
		while (cur_y < SQUARE_SIZE)
		{
			if (cur_y == 0 || cur_x == 0 || cur_x + 1 == SQUARE_SIZE || cur_y + 1 == SQUARE_SIZE)
				put_pixel(&ctx->img, cur_x + x, cur_y + y, darken(color, 0.5));
			else
				put_pixel(&ctx->img, cur_x + x, cur_y + y, color);
			cur_y++;
		}
		cur_x++;
	}
}

void	draw_board(t_ctx *ctx)
{
	int	y;
	int	x;

	y = -1;
	while (++y < BOARD_SIZE)
	{
		x = -1;
		while (++x < BOARD_SIZE)
		{
			if (ctx->shm->board[y][x] == 0)
				put_square(ctx, x * 32, y * 32, 0x00111111);
			else if (ctx->shm->board[y][x] == -1)
				put_square(ctx, x * 32, y * 32, 0x00AAAA99);
			else if (ctx->shm->board[y][x] == 1)
				put_square(ctx, x * 32, y * 32, 0x0000DD22);
			else if (ctx->shm->board[y][x] == 2)
				put_square(ctx, x * 32, y * 32, 0x00DDDD22);
			else if (ctx->shm->board[y][x] == 3)
				put_square(ctx, x * 32, y * 32, 0x002200DD);
			else if (ctx->shm->board[y][x] == 4)
				put_square(ctx, x * 32, y * 32, 0x00DD0099);
			else
				put_square(ctx, x * 32, y * 32, 0x00009999);
		}
	}
}

int	loop_hook(t_ctx *ctx)
{
	sem_lock(ctx->semid);
	if (!g_running)
	{
		sem_unlock(ctx->semid);
		return (close_window(ctx));
	}

	draw_board(ctx);
	mlx_put_image_to_window(ctx->mlx, ctx->mlx_win, ctx->img.data, 0, 0);

	sem_unlock(ctx->semid);
	return (1);
}

void	set_hooks(t_ctx *ctx)
{
	mlx_hook(ctx->mlx_win, 17, 0, close_window, ctx);
	mlx_loop_hook(ctx->mlx, loop_hook, ctx);
	mlx_key_hook(ctx->mlx_win, key_hook, ctx);
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
	ctx->mlx_win = mlx_new_window(ctx->mlx, 640, 640, "lem-ipc-visualizer");
	if (!ctx->mlx_win)
		return ;
	ctx->img.data = mlx_new_image(ctx->mlx, 640, 640);
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
