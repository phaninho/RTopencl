/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_values.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/14 00:25:39 by qhonore           #+#    #+#             */
/*   Updated: 2016/12/14 00:26:39 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"
#include "libft/libft.h"

double	get_double(const char *tag, int *i, int *nb)
{
	double	val;

	val = -666;
	(*nb)--;
	if (((tag[*i] == '-' || tag[*i] == '+')
	&& ft_isdigit(tag[*i + 1])) || ft_isdigit(tag[*i]))
		val = ft_atof(&tag[*i]);
	while (tag[*i] && !ft_isspace(tag[*i]) && (tag[*i] != '\"'
	|| (*i - 1 >= 0 && ft_strprefix(&tag[*i] - 1, "\\\""))))
		(*i)++;
	return (val);
}

int		get_int(const char *tag, int *i, int *nb)
{
	int		val;

	val = -666;
	(*nb)--;
	if (((tag[*i] == '-' || tag[*i] == '+')
	&& ft_isdigit(tag[*i + 1])) || ft_isdigit(tag[*i]))
		val = ft_atoi(&tag[*i]);
	while (tag[*i] && !ft_isspace(tag[*i]) && (tag[*i] != '\"'
	|| (*i - 1 >= 0 && ft_strprefix(&tag[*i] - 1, "\\\""))))
		(*i)++;
	return (val);
}

VEC4	get_color(const char *tag, int *i, int *val)
{
	VEC4	color;

	color = (VEC4){{0.0f, 0.0f, 0.0f, 0.0f}};
	*val = 4;
	while (*val && tag[*i] && (tag[*i] != '\"'
	|| (*i - 1 >= 0 && ft_strprefix(&tag[*i] - 1, "\\\""))))
	{
		pass_spaces(tag, i);
		if (ft_isdigit(tag[*i]))
		{
			if (*val == 4)
				color.x = ft_atof(&tag[*i]) / 255.0f;
			else if (*val == 3)
				color.y = ft_atof(&tag[*i]) / 255.0f;
			else if (*val == 2)
				color.z = ft_atof(&tag[*i]) / 255.0f;
			else
				color.w = ft_atof(&tag[*i]) / 255.0f;
			(*val)--;
		}
		while (tag[*i] && !ft_isspace(tag[*i]) && (tag[*i] != '\"'
		|| (*i - 1 >= 0 && ft_strprefix(&tag[*i] - 1, "\\\""))))
			(*i)++;
	}
	return (color);
}
