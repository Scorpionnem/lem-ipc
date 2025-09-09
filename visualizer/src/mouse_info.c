/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:19:03 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/09 11:19:13 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static int	get_selected_tile(t_ctx *ctx, int x, int y)
{
	int	res;
	int	boardx;
	int	boardy;

	boardx = (int)((float)x / WIN_SIZE * BOARD_SIZE);
	boardy = (int)((float)y / WIN_SIZE * BOARD_SIZE);
	if (boardx < 0 || boardx >= BOARD_SIZE
		|| boardy < 0 || boardy >= BOARD_SIZE)
		return (0);
	res = ctx->shm->board[boardy][boardx];
	return (res);
}

int	draw_mouse_info(t_ctx *ctx, int x, int y, int selected)
{
	char	*str;
	char	*numberstr;

	mlx_mouse_get_pos(ctx->mlx, ctx->mlx_win, &x, &y);
	selected = get_selected_tile(ctx, x, y);
	if (selected != 0)
	{
		numberstr = ft_itoa(selected);
		if (!numberstr)
			return (0);
		if (selected == -1)
			str = ft_strdup("wall");
		else
			str = ft_strjoin("team: ", numberstr);
		if (!str)
		{
			free(numberstr);
			return (0);
		}
		mlx_string_put(ctx->mlx, ctx->mlx_win,
			x - (ft_strlen(str) * 5) / 2, y - 3, 0xFF0000, str);
		free(numberstr);
		free(str);
	}
	return (1);
}
