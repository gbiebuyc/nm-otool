/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 03:52:19 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/22 03:52:20 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

size_t	sizeof_header(t_data *d)
{
	return (d->is_64bit ? sizeof(struct mach_header_64) :
		sizeof(struct mach_header));
}

bool	parse_magic(t_data *d, uint32_t magic)
{
	if (!(magic == MH_MAGIC || magic == MH_CIGAM ||
		magic == MH_MAGIC_64 || magic == MH_CIGAM_64))
		return (false);
	d->is_64bit = (magic == MH_MAGIC_64 || magic == MH_CIGAM_64);
	*is_big_endian() = (magic == MH_CIGAM || magic == MH_CIGAM_64);
	return (true);
}

bool	parse_header(t_data *d, struct mach_header *header, bool inside_fat)
{
	if (parse_magic(d, header->magic))
		;
	else if (!inside_fat && header->magic == FAT_CIGAM)
		return (handle_fat(d, d->file + 8, *((uint32_t*)(d->file) + 1)));
	else
	{
		return (!ft_dprintf(d->is_otool ? STDOUT_FILENO : STDERR_FILENO,
			"%s: is not an object file\n", d->filename));
	}
	if (!parse_commands(d, d->file + sizeof_header(d), swap32(header->ncmds)) ||
		!sort_symbols(d))
		return (!ft_dprintf(2, "Invalid or corrupted file: %s\n", d->filename));
	if ((d->is_otool && !d->otool_display_arch) ||
			(d->print_filename && !inside_fat))
		print_filename_and_arch(d, NULL);
	if (d->is_otool)
		print_text_section(d);
	else
		print_symbols(d);
	return (true);
}
