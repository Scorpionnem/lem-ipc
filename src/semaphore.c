/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphore.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:34:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 10:19:54 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"

void	sem_lock(int semid)
{
	struct sembuf	sb;

	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = 0;
	semop(semid, &sb, 1);
}

void	sem_unlock(int semid)
{
	struct sembuf	sb;

	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = 0;
	semop(semid, &sb, 1);
}
