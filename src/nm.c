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

// void	parse_section(void *file, uint32_t nsects)
// {
// 	struct section_64	*section;
// 	void				*ptr;

// 	ptr = file + sizeof(struct mach_header_64);
// 	while (nsects--)
// 	{
// 		section = (struct section_64*)ptr;
// 		ft_printf("cmd size: %d\n", cmd->cmdsize);
// 		ptr = (char*)ptr + cmd->cmdsize;
// 	}
// }

// int		get_nsects(t_data *d, void *cmd)
// {
// 	int nsects;

// 	nsects = d->is_64bit ? ((struct segment_command_64*)cmd)->nsects :
// 		((struct segment_command*)cmd)->nsects;
// 	return (d->is_big_endian ? NXSwapInt(nsects) : nsects);
// }

// char	*get_section_name(t_data *d, int needle)
// {
// 	struct load_command	*cmd;
// 	int					i_sect;
// 	int					ncmds;
// 	int					nsects;

// 	cmd = d->file + d->sizeof_header;
// 	i_sect = 1;
// 	ncmds = d->ncmds;
// 	while (ncmds--)
// 	{
// 		nsects = get_nsects(d, cmd);
// 		while (nsects--)
// 		{
// 			i_sect++;
// 		}
// 		cmd = (char*)cmd + cmd->cmdsize;
// 	}
// }

size_t	sizeof_header(t_data *d)
{
	return (d->is_64bit ? sizeof(struct mach_header_64) :
		sizeof(struct mach_header));
}

char	get_type_char(t_data *d)
{
	char c;

	c = ' ';
	if ((d->sym_type & N_SECT) && d->sect_chars[d->sym_sectnum])
		c = d->sect_chars[d->sym_sectnum];
	else if ((d->sym_type & N_TYPE) == N_ABS)
		c = 'a';
	else if ((d->sym_type & N_TYPE) == N_UNDF)
		c = 'u';
	if (d->sym_type & N_EXT)
		c = ft_toupper(c);
	return (c);
}

int		parse_symbol(t_data *d, int i)
{
	struct nlist	*sym32;
	struct nlist_64	*sym64;

	if (i >= d->nsyms)
		return (0);
	if (d->is_64bit)
	{
		sym64 = ((struct nlist_64*)d->symbols) + i;
		d->sym_str = d->sym_strtab + swap32(sym64->n_un.n_strx);
		d->sym_type = sym64->n_type;
		d->sym_value = swap64(sym64->n_value);
		d->sym_sectnum = sym64->n_sect;
	}
	else
	{
		sym32 = ((struct nlist*)d->symbols) + i;
		d->sym_str = d->sym_strtab + swap32(sym32->n_un.n_strx);
		d->sym_type = sym32->n_type;
		d->sym_value = swap32(sym32->n_value);
		d->sym_sectnum = sym32->n_sect;
	}
	return (1);
}

void	print_symbols(t_data *d)
{
	int i;

	i = -1;
	while (parse_symbol(d, ++i))
	{
		if (d->sym_type & N_STAB)
			continue;
		if ((d->sym_type & N_TYPE) == N_UNDF)
			ft_printf("%*s", d->is_64bit ? 16 : 8, "");
		else
			ft_printf("%0*llx", d->is_64bit ? 16 : 8, d->sym_value);
		// ft_printf(" %b", d->sym_type);
		ft_printf(" %c %s\n", get_type_char(d), d->sym_str);
	}
}

void	swap_symbols(t_data *d, int i, int j)
{
	struct nlist	*sym32;
	struct nlist_64	*sym64;
	struct nlist	tmp32;
	struct nlist_64	tmp64;

	if (d->is_64bit)
	{
		sym64 = d->symbols;
		tmp64 = sym64[i];
		sym64[i] = sym64[j];
		sym64[j] = tmp64;
	}
	else
	{
		sym32 = d->symbols;
		tmp32 = sym32[i];
		sym32[i] = sym32[j];
		sym32[j] = tmp32;
	}
}

void	sort_symbols(t_data *d)
{
	int		i;
	char	*str;

	i = 0;
	while(parse_symbol(d, i + 1))
	{
		str = d->sym_str;
		parse_symbol(d, i);
		if (ft_strcmp(d->sym_str, str) > 0)
		{
			swap_symbols(d, i, i + 1);
			i = 0;
		}
		else
			i++;
	}
}

void	parse_symtab_command(t_data *d, struct symtab_command *cmd)
{
	if (d->symbols)
		return ;
	d->nsyms = swap32(cmd->nsyms);
	d->symbols = d->file + swap32(cmd->symoff);
	d->sym_strtab = d->file + swap32(cmd->stroff);
}

void	parse_sections(t_data *d, struct section *sect, uint32_t nsects)
{
	while (nsects--)
	{
		d->i_sect++;
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
}

void	parse_segment_command(t_data *d, void *cmd)
{
	struct segment_command		*cmd32;
	struct segment_command_64	*cmd64;

	if (d->is_64bit && (cmd64 = cmd))
		parse_sections(d, (void*)(cmd64 + 1), swap64(cmd64->nsects));
	else if ((cmd32 = cmd))
		parse_sections(d, (void*)(cmd32 + 1), swap32(cmd32->nsects));
}

int parse_commands(t_data *d, struct load_command *cmd, int ncmds)
{
	while (ncmds--)
	{
		if (swap32(cmd->cmd) == LC_SYMTAB)
			parse_symtab_command(d, (void*)cmd);
		else if (swap32(cmd->cmd) == LC_SEGMENT)
			parse_segment_command(d, (void*)cmd);
		cmd = (void*)cmd + swap32(cmd->cmdsize);
	}
	return (0);
}

bool	parse_header(t_data *d, struct mach_header *header)
{
	if (header->magic == MH_MAGIC)
		d->is_64bit = false;
	else if (header->magic == MH_MAGIC_64)
		d->is_64bit = true;
	else if (header->magic == MH_CIGAM)
		*is_big_endian() = true;
	else if (header->magic == MH_CIGAM_64)
		d->is_64bit =
			*is_big_endian() = true;
	else
		return (false);
	d->ncmds = swap32(header->ncmds);
	return (true);
}

int func(t_data *d, char **av)
{
	int			fd;
	struct stat	file_stat;

	if (!av[1])
		av[1] = "a.out";
	if ((fd = open(av[1], O_RDONLY)) < 0)
		return (ft_printf("open error: %s\n", av[1]));
	if (fstat(fd, &file_stat) != 0)
		return (ft_printf("fstat error: %s\n", av[1]));
	if (file_stat.st_size == 0)
		return (ft_printf("Invalid file: %s\n", av[1]));
	d->file = mmap(0, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (d->file == MAP_FAILED)
		return (ft_printf("mmap error\n"));
	if (!parse_header(d, d->file))
		return (ft_printf("Invalid file: %s\n", av[1]));
	parse_commands(d, d->file + sizeof_header(d), d->ncmds);
	sort_symbols(d);
	print_symbols(d);
	return (0);
}


int main(int ac, char **av)
{
	(void)ac;
	t_data d = {0};
	if (func(&d, av))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
