/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:26:58 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 11:29:22 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

int		loop_hook(t_ctx *ctx);
int		key_hook(int keycode, t_ctx *ctx);
void	handle_click(int button, int x, int y, t_ctx *ctx);

int	close_window(t_ctx *ctx)
{
	mlx_loop_end(ctx->mlx);
	return (0);
}

int	mouse_press(int button, int x, int y, t_ctx *ctx)
{
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
	ctx->mlx_win = mlx_new_window(ctx->mlx,
			WIN_SIZE, WIN_SIZE, "lem-ipc-visualizer");
	if (!ctx->mlx_win)
		return ;
	ctx->img.data = mlx_new_image(ctx->mlx, WIN_SIZE, WIN_SIZE);
	if (!ctx->img.data)
		return ;
	ctx->img.addr = mlx_get_data_addr(ctx->img.data, &ctx->img.bits_per_pixel,
			&ctx->img.line_length, &ctx->img.endian);
	set_hooks(ctx);
	free_mlx(ctx);
}
