/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 15:11:55 by lvalenti          #+#    #+#             */
/*   Updated: 2016/10/06 20:59:50 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>

void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char const *s, int fd);
char				*ft_dtoa(double n);

#endif
