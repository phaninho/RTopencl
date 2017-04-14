/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/04 13:45:36 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/14 16:21:49 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"
#include "libft/libft.h"
#include "utils/error.h"

static void	init_elements2(t_parse *p, t_env *e)
{
	if (p->type == SCENE)
	{
		p->scene++;
		p->in_scene = 1;
		if (p->scene > 1)
			die("too many scenes");
	}
	else if (p->in_scene && p->type == CAMERA)
		p->cam++;
	else if (p->in_scene && p->type >= OBJECTS && p->type <= END_OBJECTS)
		p->object++;
	else if (p->in_scene && p->type >= LIGHTS && p->type <= END_LIGHTS)
		p->light++;
	else if (p->in_scene && p->type == MATERIAL)
		p->mate++;
}

static void	init_elements(t_env *e, const char *file)
{
	int		i;
	int		j;
	char	*tag;
	t_parse	p;

	i = 0;
	init_parse(&p);
	while (file[i])
	{
		if (ft_strprefix(&file[i], "<!--"))
			i = comment(file, i);
		if (ft_strprefix(&file[i], "</scene>"))
			p.in_scene = 0;
		else if (file[i] == '<')
		{
			tag = parse_tag(file, &i);
			p.type = parse_tag_type(tag, &j);
			init_elements2(&p, e);
			free(tag);
		}
		i++;
	}
	malloc_elements(e, &p);
}

static int	parse_file2(char *file, t_env *e, int i, t_parse *p)
{
	int	j;

	j = 0;
	if (ft_strprefix(&file[i], "<!--"))
		i = comment(file, i);
	if (ft_strprefix(&file[i], "</"))
		close_tag(e, p, file, &i);
	else if (file[i] == '<')
	{
		p->tag = parse_tag(file, &i);
		p->type = parse_tag_type(p->tag, &j);
		if (p->type >= OBJECTS && p->type <= END_OBJECTS
		&& !ft_strsuffix(p->tag, "/>"))
			p->in_object = p->object + 1;
		if (p->type == SCENE)
			p->in_scene = 1;
		if (p->in_scene && p->type != -1)
		{
			p->arg = parse_args(&(p->tag[j]), p->type);
			check_values(e, p, &(p->arg));
		}
		free(p->tag);
	}
	return (i);
}

void		parse_file(t_env *e, const char *name)
{
	char	*file;
	int		i;
	t_parse	p;

	get_file(name, &file);
	init_parse(&p);
	init_scene(e);
	init_elements(e, file);
	init_camera(e);
	init_objects(e, e->scene.max_object);
	init_lights(e, e->scene.max_light);
	init_materials(e, e->scene.max_material);
	i = 0;
	while (file[i])
	{
		i = parse_file2(file, e, i, &p);
		i++;
	}
	e->scene.name = (char*)name;
	free(file);
}
