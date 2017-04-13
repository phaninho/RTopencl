/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/13 23:09:08 by stmartin          #+#    #+#             */
/*   Updated: 2017/04/13 23:12:29 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

# ifndef BUFF_SIZE
# define BUFF_SIZE (1 << 14)
# endif

void		file2str(const char *file, char **str, size_t *size);

#endif
