/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 12:32:40 by stmartin          #+#    #+#             */
/*   Updated: 2017/03/17 12:32:42 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# define TICK_PER_MS (1000.0f / 30.0f)
# define WIN_W 1280
# define WIN_H 720

# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>

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
	SDL_Rect		interface;
}				t_window;

t_window		*window_get();
int				window_init();
void			window_loop(t_window *win);
void			window_end();
void			update(void);
void			mouse_motion(Sint32 xrel, Sint32 yrel);
void			mouse_grab(int x, int y);
int				mouse_isgrab(void);
void			mouse_ungrab(void);
char			*check_name_screen(t_window *t);

#endif
