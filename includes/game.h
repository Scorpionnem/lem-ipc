/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 10:38:00 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 11:51:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "ctx.h"
# include "render.h"

typedef struct s_gameinfo
{
	int	teams;
	int	last_team_checked;
	int	biggest_teams;
	int	biggest_team;
	int	players;
}	t_gameinfo;

void		move(t_ctx *ctx);
int			game_logic(t_ctx *ctx);
int			is_dead(t_ctx *ctx);
void		kill_cell(t_ctx *ctx);
bool		is_in_bounds(t_ctx *ctx, int x, int y);
int			get_players_in_team(t_ctx *ctx, int team);

t_gameinfo	get_game_infos(t_ctx *ctx);

#endif
