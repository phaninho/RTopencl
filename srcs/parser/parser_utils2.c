/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/29 11:10:34 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/30 12:34:00 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"
#include "libft/libft.h"
#include "utils/error.h"

void	malloc_elements(t_env *e, t_parse *p)
{
	if (!p->object)
		e->objects = NULL;
	else if (!(e->objects = (t_objects*)malloc(sizeof(t_objects) * p->object)))
		die("malloc failure (e->objects)");
	if (!p->light)
		e->light = NULL;
	else if (!(e->light = (t_light*)malloc(sizeof(t_light) * p->light)))
		die("malloc failure (e->light)");
	if (!p->mate)
		e->material = NULL;
	else if (!(e->material = (t_material*)malloc(sizeof(t_material) * p->mate)))
		die("malloc failure (e->material)");
	e->scene.max_object = p->object;
	e->scene.max_light = p->light;
	e->scene.max_material = p->mate;
}

char	*parse_tag(const char *s, int *i)
{
	char	*tag;
	int		j;

	j = *i;
	while (s[j] && s[j] != '>')
	{
		if (ft_strprefix(&s[j], "<!--"))
			j = comment(s, j);
		if (s[j] == '\"')
			pass_string(s, &j);
		j++;
	}
	if (!(tag = ft_strsub(s, *i, j - *i + 1)))
		die("malloc failure");
	*i = j;
	return (tag);
}

int		parse_tag_type(const char *tag, int *i)
{
	int		type;

	type = 0;
	*i = 1;
	while (get_tag(type))
	{
		if (ft_strprefix(&tag[*i], get_tag(type))
		&& ft_isspace(tag[*i + ft_strlen(get_tag(type))]))
		{
			*i += ft_strlen(get_tag(type));
			return (type);
		}
		type++;
	}
	return (-1);
}

void	get_render_mod(t_arg *arg, const char *tag, int *i)
{
	int			j;
	char		*name;
	int			type;

	j = *i;
	type = 0;
	while (tag[*i] && (tag[*i] != '\"'
	|| (*i - 1 >= 0 && ft_strprefix(&tag[*i] - 1, "\\\""))))
		(*i)++;
	if (!(name = ft_strsub(tag, j, *i - j)))
		die("malloc failure");
	while (get_mod(type))
	{
		if (!ft_strcmp(name, get_mod(type)))
		{
			arg->mod = type + RENDERMODE;
			break ;
		}
		type++;
	}
	free(name);
}
