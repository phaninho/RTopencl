/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/30 14:39:59 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/21 17:13:13 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"
#include <math.h>

static float	soft_length(VEC3 vec)
{
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

static VEC3		soft_normalize(VEC3 vec)
{
	float	l;

	l = soft_length(vec);
	if (l == 0.0f)
		return (vec);
	vec.x /= l;
	vec.y /= l;
	vec.z /= l;
	return (vec);
}

VEC4			check_color(VEC4 color)
{
	if (color.x > 1.0f)
		color.x = 1.0f;
	if (color.x < 0.0f)
		color.x = 0.0f;
	if (color.y > 1.0f)
		color.y = 1.0f;
	if (color.y < 0.0f)
		color.y = 0.0f;
	if (color.z > 1.0f)
		color.z = 1.0f;
	if (color.z < 0.0f)
		color.z = 0.0f;
	if (color.w > 1.0f)
		color.w = 1.0f;
	if (color.w < 0.0f)
		color.w = 0.0f;
	return (color);
}

VEC3			check_normal(VEC3 normal)
{
	if (normal.x == 0.0f && normal.y == 0.0f && normal.z == 0.0f)
	{
		normal.x = 0.0f;
		normal.y = 1.0f;
		normal.z = 0.0f;
	}
	soft_normalize(normal);
	return (normal);
}

void			check_render(t_env *e, t_arg *arg)
{
	if (arg->aint[DEBUG] == 0 || arg->aint[DEBUG] == 1)
		e->scene.render_debug = arg->aint[DEBUG];
	if (arg->mod >= RENDERMODE && arg->mod <= END_RENDERMODE)
		e->scene.render_mod = arg->mod;
	e->scene.render_filter = check_color(arg->color[FILTER - COLOR]);
}
