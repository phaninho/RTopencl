/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsuffix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/07 15:31:29 by qhonore           #+#    #+#             */
/*   Updated: 2016/12/09 22:16:16 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

int		ft_strsuffix(const char *str, const char *suffix)
{
	int		i;

	if (!str || !suffix)
		return (0);
	i = ft_strlen(str) - ft_strlen(suffix);
	if (i < 0)
		return (0);
	while (str[i] && str[i] == *suffix++)
		i++;
	return (str[i] == '\0' ? 1 : 0);
}
