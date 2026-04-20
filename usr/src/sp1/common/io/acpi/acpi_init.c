/*
 * Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distrubution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

#include <io/acpi/acpi.h>
#include <io/acpi/tables.h>
#include <lib/printf.h>
#include <os/knot.h>
#include <os/bpt.h>
#include <mm/vm.h>
#include <string.h>

#define pr_trace(fmt, ...) \
    printf("acpi: " fmt, ##__VA_ARGS__)

static struct acpi_rsdp *rsdp = NULL;
static struct acpi_root_sdt *root_sdt = NULL;
static size_t root_sdt_entries = 0;

/*
 * Verify the RSDP structure
 */
static void
rsdp_verify(void)
{
    uint8_t major;
    uint8_t csum = 0;

    /*
     * Some implementations like those found in emulators may
     * have their revision set to zero alongside ACPI 1.0 behavior.
     * If we encounter a zeroed revision, just bump it up by one.
     */
    if ((major = rsdp->revision) == 0) {
        ++major;
    }

    /* Print the version and compute checksum */
    pr_trace("detected acpi %d.0 by %.6s\n", major, rsdp->oemid);
    for (size_t i = 0; i < rsdp->length; ++i) {
        csum += ((uint8_t *)rsdp)[i];
    }

    /* Is the checksum valid? */
    if ((csum & 0xFF) != 0) {
        knot("bad checksum %x for acpi rsdp\n", csum);
    }
}

void *
acpi_query(const char *signature)
{
    struct acpi_header *hdr;
    uintptr_t pma;

    if (signature == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < root_sdt_entries; ++i) {
        pma = (uintptr_t)root_sdt->tables[i];
        hdr = (struct acpi_header *)pma_to_vma(pma);
        if (memcmp(hdr->signature, signature, sizeof(hdr->signature)) == 0) {
            return (void *)hdr;
        }
    }

    return NULL;
}

void
acpi_init(void)
{
    struct bpt_protovar pv;
    status_t status;
    struct acpi_header *hdr;
    uintptr_t pma;

    status = bpt_get_protovar(&pv);
    if (status != STATUS_SUCCESS) {
        knot("could not obtain protovar\n");
    }

    rsdp = pv.rsdp;
    rsdp_verify();

    /* Select the correct root sdt */
    if (rsdp->revision < 2) {
        pr_trace("using rsdt as root sdt\n");
        root_sdt = pma_to_vma((uintptr_t)rsdp->rsdt_addr);
        root_sdt_entries = (root_sdt->hdr.length - sizeof(root_sdt->hdr)) / 4;
    } else {
        pr_trace("using xsdt as root sdt\n");
        root_sdt = pma_to_vma((uintptr_t)rsdp->xsdt_addr);
        root_sdt_entries = (root_sdt->hdr.length - sizeof(root_sdt->hdr)) / 8;
    }
}
