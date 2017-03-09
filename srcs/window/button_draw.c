/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/09 12:56:52 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/09 17:28:49 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

static void	draw_slider(t_window *win, t_button *button)
{
	SDL_Rect	rec;

	SDL_SetRenderDrawColor(win->renderer, 0x60, 0x60, 0x60, 0xff);
	SDL_RenderFillRect(win->renderer, &(button->rect));
	if (button->name && button->name[0] == 'r')
		SDL_SetRenderDrawColor(win->renderer, SLIDER_RED);
	else if (button->name && button->name[0] == 'g')
		SDL_SetRenderDrawColor(win->renderer, SLIDER_GREEN);
	else if (button->name && button->name[0] == 'b')
		SDL_SetRenderDrawColor(win->renderer, SLIDER_BLUE);
	else
		SDL_SetRenderDrawColor(win->renderer, 0xee, 0xee, 0xee, 0xff);
	rec = (SDL_Rect){button->rect.x, button->rect.y,\
							button->rect.w * button->val / 100, button->rect.h};
	SDL_RenderFillRect(win->renderer, &rec);
}

static void	draw_onoff(t_window *win, t_button *button)
{
	SDL_SetRenderDrawColor(win->renderer, 0x60, 0x60, 0x60, 0xff);
	SDL_RenderFillRect(win->renderer, &(button->rect));
	draw_text(button->name, button->rect.x + button->rect.w / 2,\
									button->rect.y + button->rect.h / 2, WHITE);
}

void		draw_buttons(void)
{
	int			i;
	t_window	*win;
	t_button	*cur;

	i = -1;
	win = window_get();
	SDL_SetRenderDrawColor(win->renderer, 0x50, 0x50, 0x50, 0xff);
	draw_rect(win->width - 290, 30, 280, 2);
	while (++i < BUTTON_NB)
	{
		cur = get_button(i);
		if (cur->type == SLIDER)
			draw_slider(win, cur);
		else
			draw_onoff(win, cur);
	}
}
