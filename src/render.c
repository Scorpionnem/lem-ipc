/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:40:31 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/03 14:46:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "game.h"

void    render_board(t_ctx *ctx)
{
	ft_putendl_fd("\033cGame Board:", 1);
	for (int y = 0; y < BOARD_SIZE; y++)
	{
		for (int x = 0; x < BOARD_SIZE; x++)
			ft_putnbr_fd(ctx->shared_memory->board[y][x], 1);
		ft_putchar_fd('\n', 1);
	}
	t_gameinfo  gameinfos = get_game_infos(ctx);
	
	ft_putnbr_fd(gameinfos.players, 1);
	ft_putendl_fd(" players", 1);

	ft_putnbr_fd(gameinfos.teams, 1);
	ft_putendl_fd(" teams", 1);

	ft_putnbr_fd(gameinfos.biggest_teams, 1);
	ft_putendl_fd(" biggest teams", 1);
}
