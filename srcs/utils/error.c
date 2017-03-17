/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 13:40:53 by mgallo            #+#    #+#             */
/*   Updated: 2016/11/30 09:31:50 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "utils/error.h"
#include "window/window.h"
#include "thread/thread.h"

void	die(const char *error)
{
	fprintf(stderr, "%s\n", error);
	window_end();
	thread_end();
	exit(-1);
}

void	opencl_error(const char *err, const void *p, size_t cb, void *u)
{
	(void)p;
	(void)cb;
	(void)u;
	fprintf(stderr, "%s\n", err);
}
