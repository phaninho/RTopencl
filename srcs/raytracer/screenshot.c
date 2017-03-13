/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screenshot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paim <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 17:05:40 by paim              #+#    #+#             */
/*   Updated: 2016/12/17 16:52:44 by paim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser/rt_parser.h"
#include "../../includes/window/window.h"
#include "../../includes/libft/libft.h"
#include "../../includes/thread/thread_SDL.h"

static int		ctoi(char c)
{
	int		i;

	if (c <= '0' && c >= '9')
		return (-10);
	i = c + 48;
	return (i);
}

static char		itoc(int n)
{
	char	c;

	c = n - 48;
	return (c);
}

static int		check_fscreen(void)
{
	if (access("./Screenshot", F_OK == -1))
		mkdir("./Screenshot", 0777);
	return (0);
}

char			*check_name_screen(t_window *t)
{
	FILE	*file;
	int		i;
	int		j;
	char	*tmp;

	check_fscreen();
	tmp = ft_strdup("Screenshot/Screen0000000.bmp");
	while ((file = fopen(tmp, "r")) != NULL)
	{
		j = 0;
		i = 23;
		if (tmp[i] == '9')
		{
			while (tmp[i] == '9')
			{
				tmp[i] = '0';
				i--;
				if (tmp[i] == '8')
				{
					j = ctoi(tmp[i]);
					j++;
					tmp[i] = itoc(j);
					break ;
				}
				if (tmp[i] != '9')
				{
					j = ctoi(tmp[i]);
					j++;
					tmp[i] = itoc(j);
				}
			}
		}
		else if (tmp[i] != '9')
		{
			j = ctoi(tmp[i]);
			j++;
			tmp[i] = itoc(j);
		}
		t->lmt_screen++;
		if (t->lmt_screen == 9999999)
			return (NULL);
		fclose(file);
	}
	return (tmp);
}
