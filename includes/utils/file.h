/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallo <mgallo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 14:00:24 by mgallo            #+#    #+#             */
/*   Updated: 2016/12/03 11:57:41 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

#ifndef BUFF_SIZE
	# define BUFF_SIZE (1 << 14)
#endif

void		file2str(const char *file, char **str, size_t *size);

#endif
