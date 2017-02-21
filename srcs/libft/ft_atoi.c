/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 19:41:34 by qhonore           #+#    #+#             */
/*   Updated: 2016/11/29 10:47:04 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

int				ft_atoi(const char *str)
{
	int		nb;
	int		neg;

	nb = 0;
	neg = 0;
	while (*str == '\n' || *str == '\v' || *str == ' '
			|| *str == '\t' || *str == '\r' || *str == '\f')
		str++;
	if (*str == '-')
		neg = 1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		nb = nb * 10 + (*str - '0');
		str++;
	}
	return (neg ? -nb : nb);
}

static double	get_decimal(const char *str)
{
	double	dec;
	long	div1;
	long	div2;

	dec = 0.0;
	div1 = 10;
	div2 = 1;
	while (*str >= '0' && *str <= '9')
	{
		dec += (double)(*str - '0') / div1 / div2;
		if (div1 < 100000000000)
			div1 *= 10;
		else if (div2 < 1000000000000)
			div2 *= 10;
		else
			break ;
		str++;
	}
	return (dec);
}

double			ft_atof(const char *str)
{
	double	nb;
	int		neg;

	nb = 0.0;
	neg = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
		neg = 1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		nb = nb * 10 + (*str - '0');
		str++;
	}
	if (*str == '.')
	{
		str++;
		nb += get_decimal(str);
	}
	return (neg ? -nb : nb);
}
