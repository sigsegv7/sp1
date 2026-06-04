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

#ifndef _PCI_PCIREGS_H_
#define _PCI_PCIREGS_H_

#include <sys/param.h>

#define PCIREG_VENDOR_ID  0x00  /* 16 bits */
#define PCIREG_DEVICE_ID  0x02  /* 16 bits */
#define PCIREG_CLASSREV   0x08  /* 32 bits */
#define PCIREG_HDRTYPE    0x0e  /* 8 bits */
#define PCIREG_BAR0       0x10  /* 32 bits */
#define PCIREG_BAR1       0x14  /* 32 bits */
#define PCIREG_BUSES      0x18  /* 24 bits */
#define PCIREG_BAR2       0x18  /* 32 bits */
#define PCIREG_BAR3       0x1C  /* 32 bits */
#define PCIREG_BAR4       0x20  /* 32 bits */
#define PCIREG_BAR5       0x24  /* 32 bits */
#define PCIREG_IRQLINE    0x3C  /* 8 bits */
#define PCIREG_CAPPTR     0x34  /* 8 bits */
#define PCIREG_CMDSTATUS  0x04  /* command (15:0), status (31:16) */

/* Macros to extract PCIREG_CLASSREV bits */
#define PCIREG_CLASS(CLASSREV) (CLASSREV >> 24)
#define PCIREG_SUBCLASS(CLASSREV) ((CLASSREV >> 16) & 0xFF)
#define PCIREG_REVID(CLASSREV) (CLASSREV & 0xFF)
#define PCIREG_PROGIF(CLASSREV) ((CLASSREV >> 8) & 0xFF)

/* Macros to extract PCI_BUSES bits */
#define PCIREG_PRIBUS(BUSES) (BUSES & 0xFF)
#define PCIREG_SECBUS(BUSES) ((BUSES >> 8) & 0xFF)
#define PCIREG_SUBBUS(BUSES) ((BUSES >> 16) & 0xFF)

/* Macros to extract PCIREG_CMDSTATUS bits */
#define PCIREG_COMMAND(CMDSTATUS) (CMDSTATUS & 0xFFFF)
#define PCIREG_STATUS(CMDSTATUS) (CMDSTATUS >> 16)

/* PCI command register bits */
#define PCI_IO_SPACE        BIT(0)  /* Respond to I/O space accesses */
#define PCI_MEM_SPACE       BIT(1)  /* Respond to mem space accesses */
#define PCI_BUS_MASTERING   BIT(2)  /* Enable bus mastering */
#define PCI_INT_DISABLE     BIT(10) /* Interrupt disable */

/* PCI status register bits */
#define PCI_STATUS_CAPLIST  BIT(4)
#define PCI_STATUS_66MHZ    BIT(5)

/* Capability IDs */
#define PCI_CAP_MSI     0x05
#define PCI_CAP_MSIX    0x11

/* PCI BAR defines */
#define PCI_BAR_TYPE(BAR) ((BAR >> 1) & 3)
#define PCI_BAR_32(BAR) (PCI_BAR_TYPE(BAR) == 0x0)
#define PCI_BAR_64(BAR) (PCI_BAR_TYPE(BAR) == 0x2)

/* PCI header types */
#define PCI_HDRTYPE_NORMAL 0x00
#define PCI_HDRTYPE_BRIDGE 0x01

#endif  /* !_PCI_PCIREGS_H_ */
