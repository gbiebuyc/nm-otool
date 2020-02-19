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
# include <mach-o/fat.h>

typedef struct				s_data
{
	char					*filename;
	void            		*file;
	struct stat				file_stat;
	uint32_t				nsyms;
	bool					is_64bit;
	size_t					i_sect;
	char					sect_chars[256];
	struct nlist_64			sym;
	struct nlist			*sym32;
	struct nlist_64			*sym64;
	char					*strtab;
	uint32_t				strsize;
	char					*sym_str;
}							t_data;

bool						*is_big_endian();
uint16_t					swap16(uint16_t n);
uint32_t					swap32(uint32_t n);
uint64_t					swap64(uint64_t n);
bool						parse_header(
	t_data *d, struct mach_header *header, bool inside_fat);

#endif
