/*
 * During the executution of a target program this will print out
 * the names of the functions used during startup. You can feed
 * the output of the function symbol column into a custom linker 
 * script so as to place the startup functions together in the
 * target library.
 *
 * 1) Compile with -finstrument-functions the program whose startup 
 * up functions you want to capture
 *
 * 2) Create a shared library from this file with...
 *
 * gcc -fpic -g instrument.c -shared -o libinstrument.so -lbfd -liberty `pkg-config --cflags --libs glib-2.0`
 *
 * 3) export LD_PRELOAD=/path/to/libinstrument.so
 *
 * 4) run your program
 *
 * The addresses of the functions called will be output on stdout in the format
 *
 * original_address relative_address dso_name symbol_name
 *
 * feel free to speed it up, it's pretty slow and dumb
 *
 */

/******************************************************************************
 * Author (c) Caolan McNamara (caolanm@redhat.com) 2007
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <link.h>

#include <bfd.h>

#include <glib.h>

static int indent = 0;

struct bfdentry
{
    unsigned int size;
    long symcount;
    asymbol **syms;
    bfd* dso;
};

static void print_indent() {
    int i;
    for (i=0; i<indent; i++) printf("  ");
}

static void print_symbol(void* this_fn, long normalized_addr, struct bfdentry *abfd, char *dsoname)
{
    bfd* dso = abfd->dso;
    unsigned int size = abfd->size;
    long symcount = abfd->symcount;
    bfd_byte *from, *fromend;

    from = (bfd_byte *)abfd->syms;
    fromend = from + abfd->symcount * abfd->size;
    for (; from < fromend; from += size)
    {  
        asymbol* store;
        asymbol* sym;
        symbol_info syminfo;

        sym = bfd_minisymbol_to_symbol(dso, FALSE, (const PTR)from, store);

        bfd_get_symbol_info(dso, sym, &syminfo);
        if (syminfo.value == normalized_addr)
        {
            const char *nm = bfd_asymbol_name(sym);
            if (nm && nm[0]) {
                indent++;
                print_indent();
                printf("%s (%p)\n", nm, this_fn);
	            //printf("%.*p %.*p %s %s\n", sizeof(char*)*2, this_fn, sizeof(char*)*2, normalized_addr, dsoname, nm);
            }
        }
    }
}

static struct bfdentry* loaddso(char *dsoname)
{
    struct bfdentry *entry;

    unsigned int size;
    long symcount;
    char **matching;
    asymbol **syms;
    bfd* dso = bfd_openr(dsoname, NULL);

    if ((dso == NULL) || (bfd_check_format(dso, bfd_archive)) || (!bfd_check_format_matches(dso, bfd_object, &matching)))
    {
        fprintf(stderr, "failure reading symbols from %s\n", dsoname);
        return NULL;
    }

    symcount = bfd_read_minisymbols(dso, FALSE, (void**)&syms, &size);
    if (!symcount)
        bfd_read_minisymbols(dso, TRUE, (void**)&syms, &size);

    entry = (struct bfdentry*)malloc(sizeof(struct bfdentry));
    entry->dso = dso;
    entry->size = size;
    entry->symcount = symcount;
    entry->syms = syms;
    return entry;
}

static void getexe(char *exe)
{
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "/proc/%d/exe", getpid());
    int len;
    if ((len = readlink(buffer, exe, PATH_MAX)) == -1)
    {
        perror(buffer);
        abort();
    }
    exe[len] = '\0';
}

void __cyg_profile_func_enter (void* this_fn, void *call_site) __attribute__ ((no_instrument_function));
void __cyg_profile_func_exit (void* this_fn, void *call_site) __attribute__ ((no_instrument_function));

static GHashTable* address_hash_table = 0;
static GHashTable* bfd_hash_table = 0;

void __cyg_profile_func_enter (void* this_fn, void *call_site)
{
    static char exe[PATH_MAX];
    int found = 0, loop = 0;
    struct link_map* map;

    if (exe[0] == 0) getexe(exe);

    if (!address_hash_table) address_hash_table = g_hash_table_new(NULL, NULL);

    // dont skip duplicates
    //if (g_hash_table_lookup(address_hash_table, this_fn)) return;

    if (!bfd_hash_table)
        bfd_hash_table = g_hash_table_new(g_str_hash, g_str_equal);

    map = _r_debug.r_map;
    while (map != NULL) {
	    if ((!loop ? map->l_addr : 1) && 
            (this_fn >= (void*)(map->l_addr)) && (this_fn <= (void*)(map->l_ld)))
        {
            struct bfdentry *abfd;
	        char *dsoname = strcmp(map->l_name, "") ? map->l_name : exe;
            long relative_address = ((long)(this_fn) - map->l_addr);

            g_hash_table_insert(address_hash_table, this_fn, (gpointer)relative_address);

            abfd = (struct bfdentry*)(g_hash_table_lookup(bfd_hash_table, dsoname));

            if (!abfd)
            {
                g_hash_table_insert(bfd_hash_table, dsoname, loaddso(dsoname));
                abfd = (struct bfdentry*)(g_hash_table_lookup(bfd_hash_table, dsoname));
            }

            print_symbol(this_fn, relative_address, abfd, dsoname);

            found = 1;
            break;
        }
	    map = map->l_next;
        if (!map && !found && !loop)
        {
            ++loop;
            map = _r_debug.r_map;
        }
    }
}

void __cyg_profile_func_exit (void* this_fn, void *call_site) {
    indent--;
}




