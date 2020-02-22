/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 03:56:18 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/22 03:56:19 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

bool	parse_segment_command(t_data *d, struct segment_command *cmd)
{
	return (parse_sections(d, (void*)(cmd + 1), swap32(cmd->nsects)));
}

bool	parse_segment_command_64(t_data *d, struct segment_command_64 *cmd)
{
	return (parse_sections(d, (void*)(cmd + 1), swap32(cmd->nsects)));
}

bool	parse_symtab_command(t_data *d, struct symtab_command *cmd)
{
	if (swap32(cmd->stroff) + swap32(cmd->strsize) > d->file_stat.st_size)
		return (false);
	if (swap32(cmd->symoff) +
		swap32(cmd->nsyms) * sizeof_symbol(d) > (uint64_t)d->file_stat.st_size)
		return (false);
	d->strtab = d->file + swap32(cmd->stroff);
	d->sym32 = d->file + swap32(cmd->symoff);
	d->sym64 = d->file + swap32(cmd->symoff);
	d->nsyms = swap32(cmd->nsyms);
	d->strsize = swap32(cmd->strsize);
	return (true);
}

bool	parse_commands(t_data *d, struct load_command *cmd, int ncmds)
{
	while (ncmds--)
	{
		cmd->cmd = swap32(cmd->cmd);
		cmd->cmdsize = swap32(cmd->cmdsize);
		if (((void*)cmd + cmd->cmdsize) > (d->file + d->file_stat.st_size))
			return (false);
		if (cmd->cmd == LC_SYMTAB)
		{
			if (!parse_symtab_command(d, (void*)cmd))
				return (false);
		}
		else if (cmd->cmd == LC_SEGMENT)
		{
			if (!parse_segment_command(d, (void*)cmd))
				return (false);
		}
		else if (cmd->cmd == LC_SEGMENT_64)
		{
			if (!parse_segment_command_64(d, (void*)cmd))
				return (false);
		}
		cmd = (void*)cmd + cmd->cmdsize;
	}
	return (true);
}
