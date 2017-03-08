/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_text.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 20:25:43 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/08 13:19:34 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include "window/window.h"
#include "window/interface.h"
#include "raytracer/rt_env.h"

t_font				*get_font(void)
{
	static t_font	font;

	return (&font);
}

void				init_font(void)
{
	t_font		*font;
	t_window	*win;

	font = get_font();
	win = window_get();
	TTF_Init();
	font->font_size = 15;
	font->font = TTF_OpenFont("/Library/Fonts/Arial Bold.ttf", font->font_size);
}

static SDL_Texture	*surface_to_texture(t_window *win, t_font *font)
{
	SDL_Texture	*text;

	text = SDL_CreateTextureFromSurface(win->renderer, font->surf);
	SDL_FreeSurface(font->surf);
	return (text);
}

void				draw_text(char *str, int x, int y, SDL_Color color)
{
	t_window	*win;
	t_font		*font;

	win = window_get();
	font = get_font();
	font->surf = TTF_RenderText_Solid(font->font, str, color);
	font->texture = surface_to_texture(win, font);
	font->rect.x = x;
	font->rect.y = y;
	SDL_QueryTexture(font->texture, NULL, NULL, &font->rect.w, &font->rect.h);
	SDL_RenderCopy(win->renderer, font->texture, NULL, &font->rect);
	SDL_DestroyTexture(font->texture);
}
