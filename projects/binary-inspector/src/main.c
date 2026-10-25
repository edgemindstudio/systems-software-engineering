#include "binspect/elf_view.h"

#include <elf.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *etype_name(uint16_t type)
{
    switch (type) {
    case ET_REL: return "REL";
    case ET_EXEC: return "EXEC";
    case ET_DYN: return "DYN";
    case ET_CORE: return "CORE";
    default: return "OTHER";
    }
}

static const char *phdr_type(uint32_t type)
{
    switch (type) {
    case PT_LOAD: return "LOAD";
    case PT_DYNAMIC: return "DYNAMIC";
    case PT_INTERP: return "INTERP";
    case PT_NOTE: return "NOTE";
    case PT_PHDR: return "PHDR";
    case PT_TLS: return "TLS";
    default: return "OTHER";
    }
}

static int cmd_summary(const elf_view_t *view)
{
    const Elf64_Ehdr *e = view->ehdr;
    printf("class=ELF64\n");
    printf("endianness=little\n");
    printf("type=%s\n", etype_name(e->e_type));
    printf("machine=%u\n", (unsigned)e->e_machine);
    printf("entry=0x%" PRIx64 "\n", (uint64_t)e->e_entry);
    printf("program_headers=%u\n", (unsigned)e->e_phnum);
    printf("section_headers=%u\n", (unsigned)e->e_shnum);
    return 0;
}

static int cmd_sections(const elf_view_t *view)
{
    for (size_t i = 0U; i < view->ehdr->e_shnum; ++i) {
        const Elf64_Shdr *s = elf_view_section(view, i);
        const char *name = elf_view_section_name(view, i);
        if (s == NULL || name == NULL) {
            fprintf(stderr, "invalid section table\n");
            return 1;
        }
        printf("%zu\t%s\ttype=%u\toffset=0x%" PRIx64
               "\tsize=%" PRIu64 "\taddr=0x%" PRIx64 "\n",
               i, name, (unsigned)s->sh_type,
               (uint64_t)s->sh_offset,
               (uint64_t)s->sh_size,
               (uint64_t)s->sh_addr);
    }
    return 0;
}

static int cmd_segments(const elf_view_t *view)
{
    for (size_t i = 0U; i < view->ehdr->e_phnum; ++i) {
        const Elf64_Phdr *p = elf_view_segment(view, i);
        if (p == NULL) {
            return 1;
        }
        printf("%zu\t%s\toffset=0x%" PRIx64
               "\tvaddr=0x%" PRIx64
               "\tfilesz=%" PRIu64
               "\tmemsz=%" PRIu64
               "\tflags=0x%x\n",
               i, phdr_type(p->p_type),
               (uint64_t)p->p_offset,
               (uint64_t)p->p_vaddr,
               (uint64_t)p->p_filesz,
               (uint64_t)p->p_memsz,
               (unsigned)p->p_flags);
    }
    return 0;
}

static int section_bounds_ok(const elf_view_t *view, const Elf64_Shdr *s)
{
    return s->sh_offset <= view->size &&
           s->sh_size <= (uint64_t)view->size - s->sh_offset;
}

static int cmd_symbols(const elf_view_t *view)
{
    for (size_t i = 0U; i < view->ehdr->e_shnum; ++i) {
        const Elf64_Shdr *s = elf_view_section(view, i);
        if (s == NULL ||
            (s->sh_type != SHT_SYMTAB && s->sh_type != SHT_DYNSYM)) {
            continue;
        }

        const Elf64_Shdr *strs = elf_view_section(view, s->sh_link);
        if (strs == NULL || !section_bounds_ok(view, s) ||
            !section_bounds_ok(view, strs) ||
            s->sh_entsize != sizeof(Elf64_Sym)) {
            fprintf(stderr, "invalid symbol table\n");
            return 1;
        }

        size_t count = (size_t)(s->sh_size / sizeof(Elf64_Sym));
        const Elf64_Sym *syms =
            (const Elf64_Sym *)(view->data + s->sh_offset);
        const char *strtab =
            (const char *)(view->data + strs->sh_offset);

        for (size_t j = 0U; j < count; ++j) {
            if (syms[j].st_name >= strs->sh_size) {
                fprintf(stderr, "invalid symbol name\n");
                return 1;
            }
            const char *name = strtab + syms[j].st_name;
            size_t remaining =
                (size_t)(strs->sh_size - syms[j].st_name);
            if (memchr(name, '\0', remaining) == NULL) {
                fprintf(stderr, "unterminated symbol name\n");
                return 1;
            }
            if (*name != '\0') {
                printf("%s\tvalue=0x%" PRIx64
                       "\tsize=%" PRIu64
                       "\tbind=%u\ttype=%u\n",
                       name,
                       (uint64_t)syms[j].st_value,
                       (uint64_t)syms[j].st_size,
                       (unsigned)ELF64_ST_BIND(syms[j].st_info),
                       (unsigned)ELF64_ST_TYPE(syms[j].st_info));
            }
        }
    }
    return 0;
}

static int cmd_needed(const elf_view_t *view)
{
    for (size_t i = 0U; i < view->ehdr->e_shnum; ++i) {
        const Elf64_Shdr *s = elf_view_section(view, i);
        if (s == NULL || s->sh_type != SHT_DYNAMIC) {
            continue;
        }

        const Elf64_Shdr *strs = elf_view_section(view, s->sh_link);
        if (strs == NULL || !section_bounds_ok(view, s) ||
            !section_bounds_ok(view, strs) ||
            s->sh_entsize != sizeof(Elf64_Dyn)) {
            fprintf(stderr, "invalid dynamic section\n");
            return 1;
        }

        const Elf64_Dyn *dyn =
            (const Elf64_Dyn *)(view->data + s->sh_offset);
        size_t count = (size_t)(s->sh_size / sizeof(Elf64_Dyn));
        const char *strtab =
            (const char *)(view->data + strs->sh_offset);

        for (size_t j = 0U; j < count; ++j) {
            if (dyn[j].d_tag == DT_NEEDED) {
                uint64_t off = dyn[j].d_un.d_val;
                if (off >= strs->sh_size) {
                    return 1;
                }
                const char *name = strtab + off;
                size_t remaining = (size_t)(strs->sh_size - off);
                if (memchr(name, '\0', remaining) == NULL) {
                    return 1;
                }
                printf("%s\n", name);
            }
        }
    }
    return 0;
}

static int parse_u64(const char *text, uint64_t *value)
{
    char *end = NULL;
    errno = 0;
    unsigned long long v = strtoull(text, &end, 0);
    if (errno != 0 || end == text || *end != '\0') {
        return -1;
    }
    *value = (uint64_t)v;
    return 0;
}

static void usage(void)
{
    fprintf(stderr,
        "usage: binspect FILE COMMAND [VALUE]\n"
        "commands: summary sections segments symbols needed "
        "offset-to-vaddr vaddr-to-offset\n");
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        usage();
        return 2;
    }

    elf_view_t view;
    if (elf_view_open(argv[1], &view) != 0) {
        fprintf(stderr, "binspect: invalid or unsupported ELF file\n");
        return 1;
    }

    int rc = 0;

    if (strcmp(argv[2], "summary") == 0 && argc == 3) {
        rc = cmd_summary(&view);
    } else if (strcmp(argv[2], "sections") == 0 && argc == 3) {
        rc = cmd_sections(&view);
    } else if (strcmp(argv[2], "segments") == 0 && argc == 3) {
        rc = cmd_segments(&view);
    } else if (strcmp(argv[2], "symbols") == 0 && argc == 3) {
        rc = cmd_symbols(&view);
    } else if (strcmp(argv[2], "needed") == 0 && argc == 3) {
        rc = cmd_needed(&view);
    } else if ((strcmp(argv[2], "offset-to-vaddr") == 0 ||
                strcmp(argv[2], "vaddr-to-offset") == 0) &&
               argc == 4) {
        uint64_t input = 0U;
        uint64_t output = 0U;
        if (parse_u64(argv[3], &input) != 0) {
            rc = 2;
        } else if (strcmp(argv[2], "offset-to-vaddr") == 0) {
            if (elf_view_offset_to_vaddr(&view, input, &output)) {
                printf("0x%" PRIx64 "\n", output);
            } else {
                fprintf(stderr, "offset is not mapped by a PT_LOAD segment\n");
                rc = 1;
            }
        } else {
            if (elf_view_vaddr_to_offset(&view, input, &output)) {
                printf("0x%" PRIx64 "\n", output);
            } else {
                fprintf(stderr, "virtual address is not file-backed by a PT_LOAD segment\n");
                rc = 1;
            }
        }
    } else {
        usage();
        rc = 2;
    }

    elf_view_close(&view);
    return rc;
}
