/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 13:06:38 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/01 19:03:31 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"
#include "parser/rt_parser.h"

t_interface	*get_interface(void)
{
	return (&(env_get()->interface));
}

void		update_showed(t_interface *inter, int type)
{
	int		i;

	inter->index = 0;
	i = -1;
	while (get_button(++i)->exec)
		if (get_button(i)->type == inter->type)
		{
			get_button(i)->show = 0;
			get_button(i)->on = 0;
		}
	inter->type = type;
	i = -1;
	while (get_button(++i)->exec)
		if (get_button(i)->type == inter->type)
			get_button(i)->show = 1;
}

void		check_vec3(VEC3 *vec, VEC3 max, VEC3 min)
{
	if (vec->x > max.x)
		vec->x = max.x;
	else if (vec->x < min.x)
		vec->x = min.x;
	if (vec->y > max.y)
		vec->y = max.y;
	else if (vec->y < min.y)
		vec->y = min.y;
	if (vec->z > max.z)
		vec->z = max.z;
	else if (vec->z < min.z)
		vec->z = min.z;
}

void		check_float(float *nb, float max, float min)
{
	if (*nb > max)
		*nb = max;
	else if (*nb < min)
		*nb = min;
}

void		check_int(int *nb, int max, int min)
{
	if (*nb > max)
		*nb = max;
	else if (*nb < min)
		*nb = min;
}
