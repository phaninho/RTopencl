/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 11:10:33 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/11 12:29:36 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/types.h>

char	*ft_strdup(const char *s1);
int		ft_atoi(const char *str);
double	ft_atof(const char *str);
char	*ft_itoa(int nbr);
char	*ft_ltoa(long nbr);
size_t	ft_strlen(const char *s);
int		ft_putstr(char const *s);
void	ft_putendl(char const *s);
int		ft_isspace(int c);
int		ft_isdigit(int c);
int		ft_strprefix(const char *str, const char *prefix);
int		ft_strsuffix(const char *str, const char *suffix);
char	*ft_strsub(char const *s, unsigned int start, size_t len);
int		ft_strcmp(const char *s1, const char *s2);

#endif
