/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphore.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 10:08:11 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/07 10:42:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

int	sem_lock(int semid)
{
	struct sembuf lock;
	
	lock.sem_num = 0;
	lock.sem_op = -1;
	lock.sem_flg = 0;

	if (semop(semid, &lock, 1) == -1)
		return (error_int("Failed to execute semaphore lock operation"));
	return (1);
}

int	sem_unlock(int semid)
{
	struct sembuf lock;
	
	lock.sem_num = 0;
	lock.sem_op = 1;
	lock.sem_flg = 0;

	if (semop(semid, &lock, 1) == -1)
		return (error_int("Failed to execute semaphore unlock operation"));
	return (1);
}
