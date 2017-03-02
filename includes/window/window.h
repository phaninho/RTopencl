/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 13:07:00 by mgallo            #+#    #+#             */
/*   Updated: 2017/03/02 20:31:30 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# define TICK_PER_MS (1000.0f / 30.0f)
# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>

typedef struct	s_font
{
	uint8_t			font_size;
	TTF_Font		*font;
	SDL_Surface		*surf;
	SDL_Texture		*texture;
	SDL_Rect		rect;
}				t_font;


typedef struct	s_window
{
	SDL_Window		*win;
	SDL_Surface		*surface;
	SDL_Renderer	*renderer;
	int				close_request;
	int				width;
	int				height;
	unsigned char	*pixels;
	int				keys[SDL_NUM_SCANCODES];
	int				timer;
	int				frame;
	int				tick;
	int				time_tick;
	float			time_delta;
	float			move_speed;
	float			rot_speed;
	float			xrel;
	float			yrel;
	void			(*render)(void);
	void			(*update)(void);
	int				lmt_screen;
}				t_window;

t_window		*window_get();
int				window_init();
void			window_loop();
void			window_end();
void			update(void);
void			mouse_motion(Sint32 xrel, Sint32 yrel);
void			mouse_grab(void);
int				mouse_isgrab(void);
void			mouse_ungrab(void);
char			*check_name_screen(t_window *t);

t_font 			*get_font(void);
void			init_font(void);
void			draw_ui(void);
void			draw_text(char *str, int x, int y, SDL_Color color);

#endif
