/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sections.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 03:54:34 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/22 03:54:36 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

bool	check_section_valid(t_data *d,
			struct section *sect32, struct section_64 *sect64)
{
	if (d->is_64bit)
	{
		if (swap32(sect64->offset) +
			swap64(sect64->size) > (uint64_t)d->file_stat.st_size)
			return (false);
		if ((void*)(sect64 + 1) >= d->file + d->file_stat.st_size)
			return (false);
	}
	else
	{
		if (swap32(sect32->offset) +
			swap32(sect32->size) > d->file_stat.st_size)
			return (false);
		if ((void*)(sect64 + 1) >= d->file + d->file_stat.st_size)
			return (false);
	}
	return (true);
}

void	parse_text_section(t_data *d,
			struct section *sect32, struct section_64 *sect64)
{
	if (d->is_64bit)
	{
		d->text_section_vaddr = swap64(sect64->addr);
		d->text_section_size = swap64(sect64->size);
		d->text_section_addr = d->file + swap32(sect64->offset);
	}
	else
	{
		d->text_section_vaddr = swap32(sect32->addr);
		d->text_section_size = swap32(sect32->size);
		d->text_section_addr = d->file + swap32(sect32->offset);
	}
}

bool	parse_sections(t_data *d, struct section *sect, uint32_t nsects)
{
	while (nsects--)
	{
		d->i_sect++;
		if (!check_section_valid(d, sect, (void*)sect))
			return (false);
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
		{
			d->sect_chars[d->i_sect] = 't';
			parse_text_section(d, sect, (void*)sect);
		}
		else if (ft_strcmp(sect->sectname, SECT_DATA) == 0)
			d->sect_chars[d->i_sect] = 'd';
		else if (ft_strcmp(sect->sectname, SECT_BSS) == 0)
			d->sect_chars[d->i_sect] = 'b';
		else
			d->sect_chars[d->i_sect] = 's';
		sect = (void*)sect + (d->is_64bit ? sizeof(struct section_64) :
			sizeof(struct section));
	}
	return (true);
}
