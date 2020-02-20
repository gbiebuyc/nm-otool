/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 18:10:04 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/12 18:10:04 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

size_t	sizeof_header(t_data *d)
{
	return (d->is_64bit ? sizeof(struct mach_header_64) :
		sizeof(struct mach_header));
}

char	get_type_char(t_data *d)
{
	char c;

	c = '?';
	if (!d->sym.n_type)
		c = '?';
	else if ((d->sym.n_type & N_TYPE) == N_ABS)
		c = 'a';
	else if ((d->sym.n_type & N_TYPE) == N_INDR)
		c = 'i';
	else if ((d->sym.n_type & N_SECT) && d->sect_chars[d->sym.n_sect])
		c = d->sect_chars[d->sym.n_sect];
	else if ((d->sym.n_type & N_TYPE) == N_UNDF && !d->sym.n_value)
		c = 'u';
	else if ((d->sym.n_type & N_TYPE) == N_UNDF && d->sym.n_value)
		c = 'C';
	if (d->sym.n_type & N_EXT)
		c = ft_toupper(c);
	return (c);
}

bool	parse_symbol(t_data *d, int i, struct nlist_64 *sym, char **str)
{
	if (d->is_64bit)
	{
		*sym = d->sym64[i];
		sym->n_un.n_strx = swap32(sym->n_un.n_strx);
		sym->n_value = swap64(sym->n_value);
	}
	else
	{
		sym->n_un.n_strx = swap32(d->sym32[i].n_un.n_strx);
		sym->n_value = swap32(d->sym32[i].n_value);
		sym->n_type = d->sym32[i].n_type;
		sym->n_sect = d->sym32[i].n_sect;
	}
	if (sym->n_un.n_strx > d->strsize)
		*str = "bad string index";
	else
		*str = d->strtab + sym->n_un.n_strx;
	return (true);
}

void	print_symbols(t_data *d)
{
	int i;

	i = -1;
	while (++i < d->nsyms)
	{
		parse_symbol(d, i, &d->sym, &d->sym_str);
		if (d->sym.n_type & N_STAB)
			continue;
		if (d->sym.n_type &&
				(d->sym.n_type & N_TYPE) == N_UNDF && !d->sym.n_value)
			ft_printf("%*s", d->is_64bit ? 16 : 8, "");
		else
			ft_printf("%0*llx", d->is_64bit ? 16 : 8, d->sym.n_value);
		ft_printf(" %c %s\n", get_type_char(d), d->sym_str);
	}
}

void	swap_symbols(t_data *d, int i, int j)
{
	struct nlist	tmp32;
	struct nlist_64	tmp64;

	if (d->is_64bit)
	{
		tmp64 = d->sym64[i];
		d->sym64[i] = d->sym64[j];
		d->sym64[j] = tmp64;
	}
	else
	{
		tmp32 = d->sym32[i];
		d->sym32[i] = d->sym32[j];
		d->sym32[j] = tmp32;
	}
}

bool	sort_symbols(t_data *d)
{
	int		i;
	int		j;
	int		ret;

	i = -1;
	while (++i < d->nsyms - 1)
	{
		j = -1;
		while (++j < d->nsyms - i - 1)
		{
			if (!parse_symbol(d, j, &d->sym, &d->sym_str))
				return (false);
			if (d->sym.n_type & N_STAB)
				continue ;
			if (!parse_symbol(d, j + 1, &d->sym2, &d->sym_str2))
				return (false);
			if ((ret = ft_strcmp(d->sym_str, d->sym_str2)) > 0 ||
				(ret == 0 && d->sym.n_value > d->sym2.n_value))
				swap_symbols(d, j, j + 1);
		}
	}
	return (true);
}

bool	check_section_valid(t_data *d,
			struct section *sect32, struct section_64 *sect64)
{
	if (d->is_64bit)
	{
		if (swap32(sect64->offset) +
			swap64(sect64->size) > d->file_stat.st_size)
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

bool	parse_sections(t_data *d, struct section *sect, uint32_t nsects)
{
	while (nsects--)
	{
		d->i_sect++;
		if (!check_section_valid(d, sect, (void*)sect))
			return (false);
		if (ft_strcmp(sect->sectname, SECT_TEXT) == 0)
			d->sect_chars[d->i_sect] = 't';
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
	if (swap32(cmd->symoff) + swap32(cmd->nsyms) > d->file_stat.st_size)
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

char	*get_arch_name(cpu_type_t type, cpu_subtype_t subtype)
{
	if (type == CPU_TYPE_I386)
		return ("i386");
	if (type == CPU_TYPE_POWERPC && subtype != CPU_SUBTYPE_POWERPC_7400)
		return ("ppc");
	return ("");
}

bool	parse_fat(t_data *d, struct fat_arch *arch, int narch)
{
	while (narch--)
	{
		*is_big_endian() = true;
		ft_printf("\n%s (for architecture %s):\n", d->filename,
			get_arch_name(swap32(arch->cputype), swap32(arch->cpusubtype)));
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
		if ((cpu_type_t)swap32(arch->cputype) == CPU_TYPE_X86_64)
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

bool	parse_header(t_data *d, struct mach_header *header, bool inside_fat)
{
	*is_big_endian() = false;
	d->is_64bit = false;
	if (header->magic == MH_MAGIC)
		d->is_64bit = false;
	else if (header->magic == MH_MAGIC_64)
		d->is_64bit = true;
	else if (header->magic == MH_CIGAM)
		*is_big_endian() = true;
	else if (header->magic == MH_CIGAM_64)
		d->is_64bit =
			*is_big_endian() = true;
	else if (!inside_fat && header->magic == FAT_CIGAM)
		return (handle_fat(d, d->file + 8, *((uint32_t*)(d->file) + 1)));
	else
		return (false);
	if (!parse_commands(d, d->file + sizeof_header(d), swap32(header->ncmds)) ||
		!sort_symbols(d))
		return (false);
	print_symbols(d);
	return (true);
}

int	func(t_data *d, char **av)
{
	int			fd;

	d->filename = av[1];
	if (!d->filename)
		d->filename = "a.out";
	if ((fd = open(d->filename, O_RDONLY)) < 0)
		return (ft_dprintf(STDERR_FILENO, "open error: %s\n", d->filename));
	if (fstat(fd, &d->file_stat) != 0)
		return (ft_dprintf(STDERR_FILENO, "fstat error: %s\n", d->filename));
	if (d->file_stat.st_size == 0)
		return (ft_dprintf(STDERR_FILENO, "Invalid file: %s\n", d->filename));
	d->file = mmap(0, d->file_stat.st_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (d->file == MAP_FAILED)
		return (ft_dprintf(STDERR_FILENO, "mmap error\n"));
	d->file_start = d->file;
	if (!parse_header(d, d->file, false))
	{
		munmap(d->file, d->file_stat.st_size);
		return (ft_dprintf(STDERR_FILENO,
			"Invalid or corrupted file: %s\n", d->filename));
	}
	munmap(d->file, d->file_stat.st_size);
	return (0);
}

int	main(int ac, char **av)
{
	t_data d;

	(void)ac;
	d = (t_data){0};
	if (func(&d, av))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
