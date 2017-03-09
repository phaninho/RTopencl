/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 20:13:05 by qhonore           #+#    #+#             */
/*   Updated: 2016/11/29 10:47:49 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

int		ft_isdigitstr(char *s)
{
	if (!s || !(*s))
		return (0);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}

int		ft_isdigitcstr(char *s, char c)
{
	if (!s || !(*s))
		return (0);
	while (*s && *s != c)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}
