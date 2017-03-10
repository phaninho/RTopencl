/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/09 12:56:52 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/10 17:24:38 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

static void	draw_slider(t_window *win, t_slider *slider)
{
	SDL_Rect	rec;

	draw_rect(slider->rect, 0xff606060);
	if (slider->vert)
		rec = (SDL_Rect){slider->rect.x, slider->rect.y + slider->rect.h,\
						slider->rect.w, -(slider->rect.h * slider->val / 100)};
	else
		rec = (SDL_Rect){slider->rect.x, slider->rect.y,\
							slider->rect.w * slider->val / 100, slider->rect.h};
	if (slider->color)
		draw_rect(rec, slider->color);
	else
		draw_rect(rec, 0xffeeeeee);
}

static void	draw_onoff(t_window *win, t_button *button)
{
	draw_rect(button->rect, 0xff606060);
	draw_text(button->name, button->rect.x + button->rect.w / 2,\
									button->rect.y + button->rect.h / 2, WHITE);
}

void		draw_buttons(void)
{
	int			i;
	t_window	*win;

	win = window_get();
	i = -1;
	while (get_button(++i)->exec)
		draw_onoff(win, get_button(i));
	i = -1;
	while (get_slider(++i)->exec)
		draw_slider(win, get_slider(i));
}
