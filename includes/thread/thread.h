/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 13:33:49 by jeexpert          #+#    #+#             */
/*   Updated: 2017/04/13 23:02:11 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_H
# define THREAD_H

# include <OpenCL/opencl.h>

typedef struct	s_thread
{
	cl_platform_id		platform_id;
	cl_uint				platform_count;
	cl_device_id		device_id;
	cl_uint				device_count;
	cl_context			context;
	cl_command_queue	queue;
	cl_mem				mem_pixel;
	cl_mem				mem_scene;
	cl_mem				mem_camera;
	cl_mem				mem_object;
	cl_mem				mem_material;
	cl_mem				mem_light;
	cl_program			program;
	cl_kernel			kernel;
}				t_thread;

t_thread		*thread_get(void);
void			thread_init(void);
void			thread_end(void);
#endif
