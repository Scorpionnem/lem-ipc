/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:40:31 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/03 11:44:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "game.h"

void    render_board(t_ctx *ctx)
{
    printf("\033cGame Board:\n");
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
            printf("%d", ctx->shared_memory->board[y][x]);
        printf("\n");
    }
    t_gameinfo  gameinfos = get_game_infos(ctx);
	
    printf("There are %d players\n", gameinfos.players);
	printf("%d teams\n", gameinfos.teams);
    printf("%d biggest team\n", gameinfos.biggest_teams);
}
