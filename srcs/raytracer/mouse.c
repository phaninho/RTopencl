/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 10:18:10 by mgallo            #+#    #+#             */
/*   Updated: 2017/03/09 13:35:19 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"

void	mouse_motion(Sint32 xrel, Sint32 yrel)
{
	t_window	*win;

	win = window_get();
	win->xrel = (float)xrel;
	win->yrel = (float)yrel;
	if (mouse_isgrab())
		SDL_WarpMouseInWindow(win->win, win->width / 2, win->height / 2);
}

void	mouse_grab(int x, int y)
{
	t_window	*win;
	int			i;

	win = window_get();
	last_click()->x = x;
	last_click()->y = y;
	if (!in_rect(win->interface, x, y))
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_ShowCursor(SDL_DISABLE);
		mouse_motion(0, 0);
	}
	else if (!mouse_isgrab() && (i = button_clicked(x, y)) != -1)
		exec_button(i);
}

int		mouse_isgrab(void)
{
	if (SDL_GetRelativeMouseMode() == SDL_TRUE)
		return (1);
	return (0);
}

void	mouse_ungrab(void)
{
	t_window	*win;

	win = window_get();
	//SDL_SetWindowGrab(win->win, SDL_FALSE);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_ShowCursor(SDL_ENABLE);
}
