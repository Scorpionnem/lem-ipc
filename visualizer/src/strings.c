/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:07:37 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/08 11:07:56 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static int	put_string(t_ctx *ctx, char *str, int val, int yoffset)
{
	char		*itoa_str;
	char		*join_str;

	itoa_str = ft_itoa(val);
	if (!itoa_str)
		return (0);
	join_str = ft_strjoin(str, itoa_str);
	if (!join_str)
		return (0);
	mlx_string_put(ctx->mlx, ctx->mlx_win, 5, WIN_SIZE - yoffset, 0xFF0000, join_str);
	free(join_str);
	free(itoa_str);
	return (1);
}

int	put_strings(t_ctx *ctx)
{
	t_gameinfo	infos;

	infos = get_game_infos(ctx);
	if (!put_string(ctx, "players: ", infos.players, 25))
		return (0);
	if (!put_string(ctx, "teams: ", infos.teams, 15))
		return (0);
	if (!put_string(ctx, "biggest team: ", infos.biggest_teams, 5))
		return (0);
	return (1);
}
