/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:35:59 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 09:34:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"

int	error_int(char *str)
{
	ft_putstr_fd("Error: ", 1);
	ft_putendl_fd(str, 1);
	return (0);
}

void	*error_ptr(char *str)
{
	ft_putstr_fd("Error: ", 1);
	ft_putendl_fd(str, 1);
	return (NULL);
}
