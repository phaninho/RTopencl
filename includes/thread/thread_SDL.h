/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_SDL.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:34:28 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/12 13:34:30 by jeexpert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <SDL2/SDL.h>

typedef struct	s_threadSDL
{
	SDL_Thread	*thread;
}								t_threadSDL;

char	*call_me(int *i, int *j, char *tmp);
int		ctoi(char c);
char	itoc(int n);
