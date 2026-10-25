#ifndef BINSPECT_ELF_VIEW_H
#define BINSPECT_ELF_VIEW_H

#include <elf.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    unsigned char *data;
    size_t size;
    const Elf64_Ehdr *ehdr;
} elf_view_t;

int elf_view_open(const char *path, elf_view_t *view);
void elf_view_close(elf_view_t *view);

int elf_view_validate(const elf_view_t *view);
const Elf64_Shdr *elf_view_section(const elf_view_t *view, size_t index);
const Elf64_Phdr *elf_view_segment(const elf_view_t *view, size_t index);
const char *elf_view_section_name(const elf_view_t *view, size_t index);

int elf_view_offset_to_vaddr(const elf_view_t *view,
                             uint64_t offset,
                             uint64_t *vaddr_out);

int elf_view_vaddr_to_offset(const elf_view_t *view,
                             uint64_t vaddr,
                             uint64_t *offset_out);

#endif
