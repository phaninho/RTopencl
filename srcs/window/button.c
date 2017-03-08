/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/08 13:05:02 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/08 17:27:36 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

void		test_button(t_button *button)
{
	t_env		*e;

	e = env_get();
	e->scene.render_mod = e->scene.render_mod == RENDERMODE_SEPIA ? 0 : RENDERMODE_SEPIA;
	printf("Button %s clicked\n", button->name);
}

t_button	g_buttons[] =
{
	{test_button, "Test", {WIN_W - 200, 100, 100, 50}}
};

t_button	*get_button(int i)
{
	if (i >= 0 && i < BUTTON_NB)
		return (&(g_buttons[i]));
	return (NULL);
}

int			in_rect(SDL_Rect rec, int x, int y)
{
	if (x >= rec.x && x < rec.x + rec.w && y >= rec.y && y < rec.y + rec.h)
		return (1);
	return (0);
}

int			button_clicked(int x, int y)
{
	int		i;

	i = -1;
	while (++i < BUTTON_NB)
		if (in_rect(g_buttons[i].rect, x, y))
			return (i + 1);
	return (0);
}

void		draw_buttons(void)
{
	int			i;
	t_window	*win;
	t_button	*cur;

	i = -1;
	win = window_get();
	while (++i < BUTTON_NB)
	{
		cur = &(g_buttons[i]);
		SDL_SetRenderDrawColor(win->renderer, 0x60, 0x60, 0x60, 0xff);
		SDL_RenderFillRect(win->renderer, &(cur->rect));
		draw_text(cur->name, cur->rect.x + 5, cur->rect.y + 5, WHITE);
	}
}
