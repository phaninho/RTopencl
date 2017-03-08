/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 13:43:39 by mgallo            #+#    #+#             */
/*   Updated: 2016/11/26 09:17:33 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_H
# define THREAD_H

# include <OpenCL/opencl.h>

typedef struct	s_thread
{
	cl_platform_id		platform_id;
	cl_uint 			platform_count;
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
