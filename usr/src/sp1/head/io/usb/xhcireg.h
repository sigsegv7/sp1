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

#ifndef _USB_XHCIREG_H_
#define _USB_XHCIREG_H_ 1

#include <sys/types.h>
#include <sys/param.h>
#include <sys/cdefs.h>

/*
 * Host Controller Capability Registers
 *
 * See xHCI spec, section 5.3, table 5-9
 */
struct xhci_caps {
    volatile uint8_t caplength;
    volatile uint8_t reserved;
    volatile uint16_t hciversion;
    volatile uint32_t hcsparams1;
    volatile uint32_t hcsparams2;
    volatile uint32_t hcsparams3;
    volatile uint32_t hccparams1;
    volatile uint32_t dboff;
    volatile uint32_t rtsoff;
    volatile uint32_t hccparams2;
};

/*
 * Host Controller Operational Registers
 *
 * See xHCI spec, section 5.4, table 5-18
 */
struct xhci_opregs {
    volatile uint32_t usbcmd;
    volatile uint32_t usbsts;
    volatile uint32_t pagesize;
    volatile uint32_t reserved;
    volatile uint32_t reserved1;
    volatile uint32_t dnctrl;
    volatile uint64_t cmd_ring;
    volatile uint32_t reserved2[4];
    volatile uint64_t dcbaa_ptr;
    volatile uint32_t config;
};

/* USBCMD bits */
#define USBCMD_RUN      BIT(0)    /* Run/stop */
#define USBCMD_HCRST    BIT(1)    /* xHC reset */
#define USBCMD_INTE     BIT(2)    /* Interrupt Enable */

/* USBSTS bits */
#define USBSTS_HCH      BIT(0)    /* HC halted */
#define USBSTS_CNR      BIT(11)   /* Controller not ready */

/* CAPS.HCSPARAMS1 fields */
#define XHCI_MAXSLOTS(HCSPARAMS1) (HCSPARAMS1 & 0xFF)
#define XHCI_MAXPORTS(HCSPARAMS1) ((HCSPARAMS1 >> 24) & 0xFF)
#define XHCI_ECP(HCCPARAMS1) ((HCCPARAMS1 >> 16) & 0xFFFF)

/* CAPS.HCSPARAMS2 fields */
#define XHCI_MAX_SP_HI(HCSPARAMS2) ((HCSPARAMS2 >> 25) & 0x1F)
#define XHCI_MAX_SP_LO(HCSPARAMS2) ((HCSPARAMS2 >> 31) & 0x1F)

#endif  /* !_USB_XHCIREG_H_ */
