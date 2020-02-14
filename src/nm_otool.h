/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 18:07:14 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/12 18:07:14 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include <libft.h>
# include <stddef.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>

typedef struct		s_data
{
	void            *file;
	uint32_t		ncmds;
	bool			is_64bit;
	void			*sym;
	uint64_t		sym_value;
	char			sym_type;
	char			*sym_strtab;
	char			*sym_str;

}					t_data;

bool				*is_big_endian();
int16_t				swap16(int16_t n);
int32_t				swap32(int32_t n);
int64_t				swap64(int64_t n);

#endif
