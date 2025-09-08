/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:16:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 11:25:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static void	put_pixel(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

static int	darken(int color, float factor)
{
	int	r;
	int	g;
	int	b;

	r = ((color >> 16) & 0xFF) * factor;
	g = ((color >> 8) & 0xFF) * factor;
	b = (color & 0xFF) * factor;
	return ((r << 16) | (g << 8) | b);
}

static void	put_square(t_ctx *ctx, int x, int y, int color)
{
	int	cur_x;
	int	cur_y;

	cur_x = 0;
	while (cur_x < SQUARE_SIZE)
	{
		cur_y = 0;
		while (cur_y < SQUARE_SIZE)
		{
			if (cur_y == 0 || cur_x == 0
				|| cur_x + 1 == SQUARE_SIZE || cur_y + 1 == SQUARE_SIZE)
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
