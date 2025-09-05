/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphore.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 10:10:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 10:10:37 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEMAPHORE_H
# define SEMAPHORE_H

int	sem_lock(int semid);
int	sem_unlock(int semid);

#endif
