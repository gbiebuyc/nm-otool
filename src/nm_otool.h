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
# include <sys/mman.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>

typedef struct				s_data
{
	char					*filename;
	void					*file;
	void					*file_start;
	struct stat				file_stat;
	int64_t					nsyms;
	bool					is_64bit;
	size_t					i_sect;
	char					sect_chars[256];
	struct nlist_64			sym;
	struct nlist_64			sym2;
	struct nlist			*sym32;
	struct nlist_64			*sym64;
	char					*strtab;
	uint32_t				strsize;
	char					*sym_str;
	char					*sym_str2;
	bool					print_filename;
	bool					is_otool;
	bool					otool_display_arch;
	void					*text_section_addr;
	size_t					text_section_vaddr;
	size_t					text_section_size;
}							t_data;

bool						*is_big_endian();
uint16_t					swap16(uint16_t n);
uint32_t					swap32(uint32_t n);
uint64_t					swap64(uint64_t n);
bool						parse_header(
	t_data *d, struct mach_header *header, bool inside_fat);
char						get_type_char(t_data *d);
void						print_symbols(t_data *d);
bool						sort_symbols(t_data *d);
bool						parse_header(
	t_data *d, struct mach_header *header, bool inside_fat);
bool						parse_sections(
	t_data *d, struct section *sect, uint32_t nsects);
bool						parse_commands(
	t_data *d, struct load_command *cmd, int ncmds);
void						print_text_section(t_data *d);
void						print_filename_and_arch(
	t_data *d, struct fat_arch *arch);
bool						handle_fat(
	t_data *d, struct fat_arch *arch, int narch);
int							main2(t_data *d, char *filename);
size_t						sizeof_symbol(t_data *d);

#endif
