/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:28:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/09 11:19:37 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"
#include "ctx.h"

int	g_running = 1;

int			put_strings(t_ctx *ctx);
void		free_mlx(t_ctx *ctx);
void		init_mlx(t_ctx *ctx);
int			close_window(t_ctx *ctx);
int			draw_crowns(t_ctx *ctx, int biggest_team);
int			draw_mouse_info(t_ctx *ctx, int x, int y, int selected);

void	handle_sigint(int sig)
{
	(void)sig;
	g_running = 0;
}

#define ESC_KEY 65307
#define SPACE_KEY 32
#define S_KEY 115

int	key_hook(int keycode, t_ctx *ctx)
{
	char	*str;

	if (keycode == ESC_KEY)
		close_window(ctx);
	if (keycode == SPACE_KEY)
	{
		sem_lock(ctx->semid);
		ctx->shm->paused = !ctx->shm->paused;
		sem_unlock(ctx->semid);
	}
	if (keycode == S_KEY)
	{
		str = ft_strjoin_free(ft_strdup("./lemipc "),
				ft_itoa(ctx->team_selected));
		str = ft_strjoin_free(str, ft_strdup(" &"));
		if (system(str) < 0)
		{
			free(str);
			return (error_int("Failed to execute system"));
		}
		free(str);
	}
	return (0);
}

int	loop_hook(t_ctx *ctx)
{
	t_gameinfo	infos;

	sem_lock(ctx->semid);
	infos = get_game_infos(ctx);
	draw_board(ctx);
	mlx_put_image_to_window(ctx->mlx, ctx->mlx_win, ctx->img.data, 0, 0);
	draw_crowns(ctx, infos.biggest_team);
	if (!put_strings(ctx) || !draw_mouse_info(ctx, 0, 0, 0) || !g_running)
	{
		sem_unlock(ctx->semid);
		return (close_window(ctx));
	}
	sem_unlock(ctx->semid);
	return (1);
}

void	handle_click(int x, int y, t_ctx *ctx)
{
	sem_lock(ctx->semid);
	x = (float)x / WIN_SIZE * BOARD_SIZE;
	y = (float)y / WIN_SIZE * BOARD_SIZE;
	if (y >= BOARD_SIZE || x >= BOARD_SIZE)
		return ;
	if (ctx->shm->board[y][x] == -1)
		ctx->shm->board[y][x] = 0;
	else if (ctx->shm->board[y][x] == 0)
		ctx->shm->board[y][x] = -1;
	sem_unlock(ctx->semid);
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
