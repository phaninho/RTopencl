/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 13:06:38 by qhonore           #+#    #+#             */
/*   Updated: 2017/04/01 19:01:32 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"
#include "parser/rt_parser.h"

t_vec2i		*last_click(void)
{
	static t_vec2i	pos = (t_vec2i){0, 0};

	return (&pos);
}

VEC4		to_255(VEC4 color)
{
	color.x *= 255;
	color.y *= 255;
	color.z *= 255;
	color.w *= 255;
	return (color);
}

void		slider_move(t_slider *s)
{
	t_vec2i		*vec;
	SDL_Rect	rec;

	vec = last_click();
	if (s->vert)
		s->val = (vec->y - s->rect.y - 1) * 100 / s->rect.h;
	else
		s->val = (vec->x - s->rect.x) * 100 / s->rect.w;
}

void		update_onoff(t_button *button)
{
	int		i;

	if (button->type)
	{
		if (button->on)
			button->on = 0;
		else
		{
			i = -1;
			while (get_button(++i)->exec)
				if (get_button(i)->type == button->type)
					get_button(i)->on = 0;
			button->on = 1;
		}
	}
}

t_button	*get_on_button(int type)
{
	int		i;

	i = -1;
	while (get_button(++i)->exec)
		if (get_button(i)->type == type && get_button(i)->on)
			return (get_button(i));
	return (NULL);
}
