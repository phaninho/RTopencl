/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/09 11:19:57 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/09 14:07:48 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

void		slider_move(t_button *button)
{
	t_vec2i		*vec;

	vec = last_click();
	button->val = (vec->x - button->rect.x) * 100 / button->rect.w;
}

void		button_render_mod(t_button *button)
{
	int		*mod;

	mod = &(env_get()->scene.render_mod);
	if (button->id == 1)
		*mod = (*mod == RENDERMODE_SEPIA ? 0 : RENDERMODE_SEPIA);
	else if (button->id == 2)
		*mod = (*mod == RENDERMODE_GRIS ? 0 : RENDERMODE_GRIS);
	else if (button->id == 3)
		*mod = (*mod == RENDERMODE_FILTER ? 0 : RENDERMODE_FILTER);
	else if (button->id == 4)
		*mod = (*mod == RENDERMODE_ADD ? 0 : RENDERMODE_ADD);
}

void		slider_render_mod(t_button *button)
{
	VEC4		*filter;

	filter = &(env_get()->scene.render_filter);
	slider_move(button);
	if (button->id == 1)
		filter->x = button->val;
	else if (button->id == 2)
		filter->y = button->val;
	else if (button->id == 3)
		filter->z = button->val;
}
