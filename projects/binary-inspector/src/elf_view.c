#include "binspect/elf_view.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int range_valid(size_t total, uint64_t offset, uint64_t size)
{
    if (offset > (uint64_t)total) {
        return 0;
    }
    return size <= (uint64_t)total - offset;
}

int elf_view_open(const char *path, elf_view_t *view)
{
    (void)memset(view, 0, sizeof *view);

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) != 0 || st.st_size < 0) {
        (void)close(fd);
        return -1;
    }

    size_t size = (size_t)st.st_size;
    unsigned char *data = malloc(size == 0U ? 1U : size);
    if (data == NULL) {
        (void)close(fd);
        return -1;
    }

    size_t done = 0U;
    while (done < size) {
        ssize_t n = read(fd, data + done, size - done);
        if (n <= 0) {
            free(data);
            (void)close(fd);
            return -1;
        }
        done += (size_t)n;
    }

    (void)close(fd);

    view->data = data;
    view->size = size;
    view->ehdr = size >= sizeof(Elf64_Ehdr) ? (const Elf64_Ehdr *)data : NULL;

    if (!elf_view_validate(view)) {
        elf_view_close(view);
        return -1;
    }

    return 0;
}

void elf_view_close(elf_view_t *view)
{
    if (view == NULL) {
        return;
    }
    free(view->data);
    (void)memset(view, 0, sizeof *view);
}

int elf_view_validate(const elf_view_t *view)
{
    if (view == NULL || view->ehdr == NULL) {
        return 0;
    }

    const Elf64_Ehdr *e = view->ehdr;

    if (e->e_ident[EI_MAG0] != ELFMAG0 ||
        e->e_ident[EI_MAG1] != ELFMAG1 ||
        e->e_ident[EI_MAG2] != ELFMAG2 ||
        e->e_ident[EI_MAG3] != ELFMAG3) {
        return 0;
    }

    if (e->e_ident[EI_CLASS] != ELFCLASS64 ||
        e->e_ident[EI_DATA] != ELFDATA2LSB ||
        e->e_ident[EI_VERSION] != EV_CURRENT ||
        e->e_version != EV_CURRENT) {
        return 0;
    }

    if (e->e_ehsize != sizeof(Elf64_Ehdr)) {
        return 0;
    }

    if (e->e_phnum > 0U) {
        if (e->e_phentsize != sizeof(Elf64_Phdr) ||
            !range_valid(view->size, e->e_phoff,
                         (uint64_t)e->e_phnum * sizeof(Elf64_Phdr))) {
            return 0;
        }
    }

    if (e->e_shnum > 0U) {
        if (e->e_shentsize != sizeof(Elf64_Shdr) ||
            !range_valid(view->size, e->e_shoff,
                         (uint64_t)e->e_shnum * sizeof(Elf64_Shdr))) {
            return 0;
        }
        if (e->e_shstrndx >= e->e_shnum && e->e_shstrndx != SHN_UNDEF) {
            return 0;
        }
    }

    return 1;
}

const Elf64_Shdr *elf_view_section(const elf_view_t *view, size_t index)
{
    if (!elf_view_validate(view) || index >= view->ehdr->e_shnum) {
        return NULL;
    }
    const unsigned char *base = view->data + view->ehdr->e_shoff;
    return &((const Elf64_Shdr *)base)[index];
}

const Elf64_Phdr *elf_view_segment(const elf_view_t *view, size_t index)
{
    if (!elf_view_validate(view) || index >= view->ehdr->e_phnum) {
        return NULL;
    }
    const unsigned char *base = view->data + view->ehdr->e_phoff;
    return &((const Elf64_Phdr *)base)[index];
}

const char *elf_view_section_name(const elf_view_t *view, size_t index)
{
    if (!elf_view_validate(view) ||
        view->ehdr->e_shstrndx == SHN_UNDEF) {
        return NULL;
    }

    const Elf64_Shdr *strtab =
        elf_view_section(view, view->ehdr->e_shstrndx);
    const Elf64_Shdr *section = elf_view_section(view, index);

    if (strtab == NULL || section == NULL ||
        !range_valid(view->size, strtab->sh_offset, strtab->sh_size) ||
        section->sh_name >= strtab->sh_size) {
        return NULL;
    }

    const char *base = (const char *)(view->data + strtab->sh_offset);
    const char *name = base + section->sh_name;
    size_t remaining = (size_t)(strtab->sh_size - section->sh_name);

    if (memchr(name, '\0', remaining) == NULL) {
        return NULL;
    }

    return name;
}

int elf_view_offset_to_vaddr(const elf_view_t *view,
                             uint64_t offset,
                             uint64_t *vaddr_out)
{
    for (size_t i = 0U; i < view->ehdr->e_phnum; ++i) {
        const Elf64_Phdr *p = elf_view_segment(view, i);
        if (p != NULL && p->p_type == PT_LOAD &&
            offset >= p->p_offset &&
            offset < p->p_offset + p->p_filesz) {
            *vaddr_out = p->p_vaddr + (offset - p->p_offset);
            return 1;
        }
    }
    return 0;
}

int elf_view_vaddr_to_offset(const elf_view_t *view,
                             uint64_t vaddr,
                             uint64_t *offset_out)
{
    for (size_t i = 0U; i < view->ehdr->e_phnum; ++i) {
        const Elf64_Phdr *p = elf_view_segment(view, i);
        if (p != NULL && p->p_type == PT_LOAD &&
            vaddr >= p->p_vaddr &&
            vaddr < p->p_vaddr + p->p_filesz) {
            *offset_out = p->p_offset + (vaddr - p->p_vaddr);
            return 1;
        }
    }
    return 0;
}
