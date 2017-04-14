/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_SDL.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:34:28 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/14 15:38:22 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SDL_H
# define THREAD_SDL_H

# include <SDL2/SDL.h>

typedef struct	s_threadsdl
{
	SDL_Thread	*thread;
}				t_threadsdl;

char			*call_me(int *i, int *j, char *tmp);
int				ctoi(char c);
char			itoc(int n);

#endif
