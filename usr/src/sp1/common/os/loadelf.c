/*
 * Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distribution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/mman.h>
#include <sys/elf.h>
#include <os/loadelf.h>
#include <lib/printf.h>
#include <string.h>
#include <stdbool.h>
#include <mu/param.h>
#include <mu/mmu.h>
#include <mm/vm.h>

#define pr_trace(fmt, ...) \
    printf("elf: " fmt, ##__VA_ARGS__)

/*
 * Ensure that an ELF binary is valid
 *
 * @eh: ELF header to check
 *
 * Returns true if valid
 */
static bool
elf64_verify(Elf64_Ehdr *eh)
{
    if (eh == NULL) {
        return false;
    }

    if (memcmp(&eh->e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0) {
        pr_trace("bad elf magic\n");
        return false;
    }

    if (eh->e_ident[EI_CLASS] != ELFCLASS64) {
        pr_trace("elf is not 64-bits\n");
        return false;
    }

    if (eh->e_ident[EI_DATA] != ELFDATA2LSB) {
        pr_trace("elf is not little-endian\n");
        return false;
    }

    if (eh->e_ident[EI_VERSION] != EV_CURRENT) {
        pr_trace("bad elf version\n");
        return false;
    }

    if (eh->e_machine != EM_X86_64) {
        pr_trace("bad elf architecture\n");
        return false;
    }

    if (eh->e_type != ET_EXEC) {
        pr_trace("elf is not executable\n");
        return false;
    }

    return true;
}

static status_t
elf64_do_load(Elf64_Ehdr *eh, struct loaded_elf *res)
{
    status_t status;
    Elf64_Phdr *phdr;
    struct mmu_vfr cur_vfr;
    struct vm_map map;
    size_t misalign, map_len;
    uintptr_t pma;
    int prot = (PROT_READ | PROT_USER);
    void *tmp_p, *data;

    if (eh == NULL || res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if (!elf64_verify(eh)) {
        return STATUS_NOT_EXEC;
    }

    mu_mmu_readvfr(&cur_vfr);

#define PHDR_I(IDX) PTR_OFFSET(eh, eh->e_phoff + (eh->e_phentsize * (IDX)))
    for (size_t i = 0; i < eh->e_phnum; ++i) {
        phdr = PHDR_I(i);
        switch (phdr->p_type) {
        case PT_LOAD:
            if (ISSET(phdr->p_flags, PF_W))
                prot |= PROT_WRITE;
            if (ISSET(phdr->p_flags, PF_X))
                prot |= PROT_EXEC;

            misalign = phdr->p_vaddr & (PAGESIZE - 1);
            map_len = ALIGN_UP(phdr->p_memsz + misalign, PAGESIZE);

            tmp_p = membox_alloc(&res->membox, map_len, MEM_TYPE_PHYSICAL);
            if (tmp_p == NULL) {
                membox_destroy(&res->membox);
                return STATUS_NO_MEMORY;
            }

            pma = vma_to_pma(tmp_p);
            map.ps = PAGESIZE_4K;
            map.vma_base = phdr->p_vaddr;
            map.pma_base = pma;
            map.length = map_len;

            status = mm_vm_map(&cur_vfr, &map, prot);
            if (status != STATUS_SUCCESS) {
                membox_destroy(&res->membox);
                return status;
            }

            data = PTR_OFFSET(eh, phdr->p_offset);
            memcpy(tmp_p, data, phdr->p_filesz);
        }
    }
#undef PHDR_I
    return STATUS_SUCCESS;
}

status_t
elf64_load(void *data, struct loaded_elf *res)
{
    status_t status;
    Elf64_Ehdr *eh;

    if (data == NULL || res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    memset(res, 0, sizeof(*res));
    status = membox_init(&res->membox);
    if (status != STATUS_SUCCESS) {
        return status;
    }

    eh = (Elf64_Ehdr *)data;
    status = elf64_do_load(eh, res);
    if (status != STATUS_SUCCESS) {
        return status;
    }

    res->entry = eh->e_entry;
    return STATUS_SUCCESS;
}
