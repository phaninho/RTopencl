/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/08 13:11:09 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/08 17:29:57 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

# define RGBA (SDL_Color)
# define DARK_GREY (SDL_Color){100, 100, 100, 255}
# define WHITE (SDL_Color){240, 0, 0, 255}
# define BUTTON_NB 1

typedef struct s_font	t_font;
typedef struct s_button	t_button;
typedef void(*t_func)(t_button*);

struct		s_font
{
	uint8_t		font_size;
	TTF_Font	*font;
	SDL_Surface	*surf;
	SDL_Texture	*texture;
	SDL_Rect	rect;
};

struct		s_button
{
	t_func		exec;
	char		*name;
	SDL_Rect	rect;
};

t_font		*get_font(void);
void		init_font(void);
void		draw_ui(void);
void		draw_text(char *str, int x, int y, SDL_Color color);

t_button	*get_button(int i);
int			in_rect(SDL_Rect rec, int x, int y);
int			button_clicked(int x, int y);
void		draw_buttons(void);

#endif
