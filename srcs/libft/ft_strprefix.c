/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strprefix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/07 13:41:12 by qhonore           #+#    #+#             */
/*   Updated: 2016/11/24 14:06:49 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_strprefix(const char *str, const char *prefix)
{
	if (!str || !prefix)
		return (0);
	while (*str && *prefix && *str == *prefix)
	{
		str++;
		prefix++;
	}
	return (!(*prefix));
}
