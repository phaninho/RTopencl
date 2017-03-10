/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/09 11:19:57 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/10 20:09:07 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

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

void		button_render_mod(void *param)
{
	int			*mod;
	t_button	*button;

	mod = &(env_get()->scene.render_mod);
	button = (t_button*)param;
	if (button->id == 1)
		*mod = (*mod == RENDERMODE_SEPIA ? 0 : RENDERMODE_SEPIA);
	else if (button->id == 2)
		*mod = (*mod == RENDERMODE_GRIS ? 0 : RENDERMODE_GRIS);
	else if (button->id == 3)
		*mod = (*mod == RENDERMODE_FILTER ? 0 : RENDERMODE_FILTER);
	else if (button->id == 4)
		*mod = (*mod == RENDERMODE_ADD ? 0 : RENDERMODE_ADD);
}

void		slider_render_mod(void *param)
{
	VEC4		*filter;
	t_slider	*slider;

	filter = &(env_get()->scene.render_filter);
	slider = (t_slider*)param;
	slider_move(slider);
	if (slider->id == 1)
		filter->x = (double)slider->val / 100.0f;
	else if (slider->id == 2)
		filter->y = (double)slider->val / 100.0f;
	else if (slider->id == 3)
		filter->z = (double)slider->val / 100.0f;
}
