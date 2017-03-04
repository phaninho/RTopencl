/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 13:29:01 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/04 18:12:23 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "raytracer/rt_env.h"

void				draw_ui(void)
{
	t_window	*win;
	t_scene		*scene;
	SDL_Rect	rect;

	win = window_get();
	scene = &(env_get()->scene);
	rect = (SDL_Rect){win->width - 300, 0, 300, win->height};
	SDL_SetRenderDrawColor(win->renderer, 0xaa, 0xaa, 0xaa, 0xff);
	SDL_RenderFillRect(win->renderer, &rect);
	draw_text("File:", win->width - 280, 15, RGBA{100, 100, 100, 255});
	draw_text(scene->name, win->width - 240, 15, RGBA{100, 100, 100, 255});
}
