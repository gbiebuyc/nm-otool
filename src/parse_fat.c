/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 04:02:16 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/22 04:02:18 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

bool	parse_fat(t_data *d, struct fat_arch *arch, int narch)
{
	d->otool_display_arch = true;
	while (narch--)
	{
		*is_big_endian() = true;
		arch->cputype = swap32(arch->cputype);
		arch->cpusubtype = swap32(arch->cpusubtype);
		print_filename_and_arch(d, arch);
		d->file = d->file_start + swap32(arch->offset);
		d->i_sect = 0;
		if (!parse_header(d, d->file, true))
			return (false);
		arch++;
	}
	return (true);
}

bool	handle_fat(t_data *d, struct fat_arch *arch, int narch)
{
	struct fat_arch *arch2;
	int				narch2;
	struct fat_arch *match_host;

	*is_big_endian() = true;
	narch = swap32(narch);
	arch2 = arch;
	narch2 = narch;
	match_host = NULL;
	while (narch--)
	{
		if (swap32(arch->offset) + swap32(arch->size) > d->file_stat.st_size)
			return (false);
		if (!match_host && (cpu_type_t)swap32(arch->cputype) == CPU_TYPE_X86_64)
			match_host = arch;
		arch++;
	}
	if (match_host)
	{
		d->file = d->file_start + swap32(match_host->offset);
		return (parse_header(d, d->file, true));
	}
	return (parse_fat(d, arch2, narch2));
}
