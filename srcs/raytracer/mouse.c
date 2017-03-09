/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 10:18:10 by mgallo            #+#    #+#             */
/*   Updated: 2016/12/03 14:40:24 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"

void	mouse_motion(Sint32 xrel, Sint32 yrel)
{
	t_window	*win;

	win = window_get();
	win->xrel = (float)xrel;
	win->yrel = (float)yrel;
	//if (SDL_GetWindowGrab(win->win) == SDL_TRUE)
	if (mouse_isgrab())
		SDL_WarpMouseInWindow(win->win, win->width / 2, win->height / 2);
}

void	mouse_grab(void)
{
	t_window	*win;

	win = window_get();
	//SDL_SetWindowGrab(win->win, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(SDL_DISABLE);
	mouse_motion(0, 0);
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
