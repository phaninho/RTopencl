/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screenshot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:42:23 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/13 22:37:42 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/thread/thread_SDL.h"

char		*call_me(int *i, int *j, char *tmp)
{
	while (tmp[*i] == '9')
	{
		tmp[*i] = '0';
		*i = *i - 1;
		if (tmp[*i] == '8')
		{
			*j = ctoi(tmp[*i]);
			*j = *j + 1;
			tmp[*i] = itoc(*j);
			break ;
		}
		if (tmp[*i] != '9')
		{
			*j = ctoi(tmp[*i]);
			*j = *j + 1;
			tmp[*i] = itoc(*j);
		}
	}
	return (tmp);
}
