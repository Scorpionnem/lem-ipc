/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:40:31 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 10:26:35 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "game.h"

#define RED "\033[31m"
#define RESET "\033[0m"
#define BLUE "\033[34m"

static void	print_game_infos(t_ctx *ctx)
{
	t_gameinfo	gameinfos;

	gameinfos = get_game_infos(ctx);
	ft_putnbr_fd(gameinfos.players, 1);
	ft_putendl_fd(" players", 1);
	ft_putnbr_fd(gameinfos.teams, 1);
	ft_putendl_fd(" teams", 1);
	ft_putnbr_fd(gameinfos.biggest_teams, 1);
	ft_putendl_fd(" biggest teams", 1);
}

void	render_board(t_ctx *ctx)
{
	int	y;
	int	x;

	ft_putendl_fd("\033cGame Board:", 1);
	y = -1;
	while (++y < BOARD_SIZE)
	{
		x = -1;
		while (++x < BOARD_SIZE)
		{
			if (ctx->shared_memory->board[y][x] != 0)
				ft_putstr_fd(RED, 1);
			if (y == ctx->pos_y && x == ctx->pos_x)
				ft_putstr_fd(BLUE, 1);
			ft_putnbr_fd(ctx->shared_memory->board[y][x], 1);
			ft_putstr_fd(RESET, 1);
		}
		ft_putchar_fd('\n', 1);
	}
	print_game_infos(ctx);
}
