/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/24 13:43:09 by qhonore           #+#    #+#             */
/*   Updated: 2016/12/14 00:29:06 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"
#include "libft/libft.h"

static int		parse_arg_type(const char *tag, int *i)
{
	int		type;

	type = 0;
	while (get_arg(type))
	{
		if (ft_strprefix(&tag[*i], get_arg(type))
		&& (ft_isspace(tag[*i + ft_strlen(get_arg(type))])
		|| tag[*i + ft_strlen(get_arg(type))] == '='))
		{
			*i += ft_strlen(get_arg(type));
			return (type);
		}
		type++;
	}
	while (tag[*i] && !ft_isspace(tag[*i]))
		(*i)++;
	return (-1);
}

static void		get_special_value(const char *tag, int *i, int type, t_arg *arg)
{
	if (type == MOD)
		get_render_mod(arg, tag, i);
}

static void		parse_arg_value(const char *tag, int *i, int type, t_arg *arg)
{
	int		val;

	val = (type < VEC ? 1 : 3);
	(*i)++;
	while (val && tag[*i] && (tag[*i] != '\"'
	|| (*i - 1 >= 0 && ft_strprefix(&tag[*i] - 1, "\\\""))))
	{
		pass_spaces(tag, i);
		if (type >= LOAD)
			get_special_value(tag, i, type, arg);
		else if (type >= COLOR)
			arg->color[type - COLOR] = get_color(tag, i, &val);
		else if (type >= VEC && val == 3)
			arg->avec[type - VEC].x = get_double(tag, i, &val);
		else if (type >= VEC && val == 2)
			arg->avec[type - VEC].y = get_double(tag, i, &val);
		else if (type >= VEC && val == 1)
			arg->avec[type - VEC].z = get_double(tag, i, &val);
		else if (type >= DOUBLE)
			arg->adbl[type - DOUBLE] = get_double(tag, i, &val);
		else if (val == 1)
			arg->aint[type] = get_int(tag, i, &val);
	}
	if (tag[*i] == '\"')
		(*i)++;
}

t_arg			parse_args(const char *tag, int arg_type)
{
	int		type;
	int		i;
	t_arg	arg;

	init_arg(&arg);
	arg.type = arg_type;
	i = 0;
	while (tag[i])
	{
		pass_spaces(tag, &i);
		if (ft_strprefix(&tag[i], "<!--"))
			i = comment(tag, i);
		if ((type = parse_arg_type(tag, &i)) != -1)
		{
			pass_spaces(tag, &i);
			if (tag[i] != '=')
				continue ;
			i++;
			pass_spaces(tag, &i);
			if (tag[i] != '\"')
				continue ;
			parse_arg_value(tag, &i, type, &arg);
		}
	}
	return (arg);
}
