/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/29 11:10:34 by qhonore           #+#    #+#             */
/*   Updated: 2016/12/16 17:43:14 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"
#include "utils/error.h"
#include "libft/libft.h"
#include <SDL2/SDL.h>

int		comment(const char *s, int i)
{
	i += 4;
	while (s[i] && !ft_strprefix(&s[i], "-->"))
		i++;
	return (i + 3);
}

void	pass_spaces(const char *s, int *i)
{
	while (ft_isspace(s[*i]))
		(*i)++;
}

void	pass_string(const char *s, int *i)
{
	(*i)++;
	while (s[*i] && (s[*i] != '\"'
	|| (*i - 1 >= 0 && ft_strprefix(&s[*i] - 1, "\\\""))))
		(*i)++;
}

void	close_tag(t_env *e, t_parse *p, char *file, int *i)
{
	int		type;

	type = 0;
	*i += 2;
	while (get_tag(type))
	{
		if (ft_strprefix(&file[*i], get_tag(type))
		&& (file[*i + ft_strlen(get_tag(type))] == '>'))
		{
			*i += ft_strlen(get_tag(type)) + 1;
			if (p->in_scene && type == SCENE)
				p->in_scene = 0;
			else if (p->in_object && type == e->objects[p->in_object - 1].type)
				p->in_object = 0;
		}
		type++;
	}
}
