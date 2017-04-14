/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 16:14:13 by qhonore           #+#    #+#             */
/*   Updated: 2017/03/21 17:21:47 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/rt_parser.h"

void	init_parse(t_parse *p)
{
	p->cam = 0;
	p->scene = 0;
	p->in_scene = 0;
	p->object = 0;
	p->light = 0;
	p->mate = 0;
	p->type = 0;
	p->in_object = 0;
	init_arg(&(p->arg));
}

void	init_arg(t_arg *arg)
{
	int	i;

	arg->type = 0;
	arg->data = NULL;
	arg->mod = 0;
	i = -1;
	while (++i != DOUBLE)
		arg->aint[i] = 0;
	i = -1;
	while (++i != VEC - DOUBLE)
		arg->adbl[i] = 0;
	i = -1;
	while (++i != COLOR - VEC)
		arg->avec[i] = (VEC3){{0.0f, 0.0f, 0.0f}};
	i = -1;
	while (++i != LOAD - COLOR)
		arg->color[i] = (VEC4){{0.0f, 0.0f, 0.0f, 0.0f}};
}
