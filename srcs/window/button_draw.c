/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/09 12:56:52 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/28 13:51:30 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

static void	draw_slider(t_window *win, t_slider *s)
{
	SDL_Rect	rec;

	rec = s->rect;
	draw_rect(rec, 0x606060);
	if (s->vert)
		rec = (SDL_Rect){rec.x, rec.y, rec.w, rec.h * s->val / 100};
	else
		rec = (SDL_Rect){rec.x, rec.y, rec.w * s->val / 100, rec.h};
	draw_rect(rec, (s->color ? s->color : DEFCOLOR));
}

static void	draw_onoff(t_window *win, t_button *button)
{
	draw_rect(button->rect, button->on ? 0xaad700 : 0x606060);
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
		if (get_button(i)->show)
			draw_onoff(win, get_button(i));
	i = -1;
	while (get_slider(++i)->exec)
		if (get_slider(i)->show)
			draw_slider(win, get_slider(i));
}
