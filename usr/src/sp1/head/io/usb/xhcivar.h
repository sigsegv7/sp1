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

#ifndef _USB_XHCIVAR_H_
#define _USB_XHCIVAR_H_ 1

#include <sys/types.h>
#include <io/usb/xhcireg.h>
#include <os/membox.h>

#define TRB_SIZE sizeof(struct xhci_trb_nop)
#define TRB_ENTRIES 32

/*
 * Represents a xHCI host controller
 *
 * @caps:      Host controller capability registers
 * @oper:      Host controller operational registers
 * @max_slots: Maximum device slots
 * @max_ports: Maximum addressable ports
 * @membox:    Per-controller memory box
 */
struct xhci_hc {
    struct xhci_caps *caps;
    struct xhci_opregs *oper;
    uint8_t max_slots;
    uint8_t max_ports;
    struct membox membox;
};

/*
 * Represents a transfer request block for NO-OP operations
 *
 * See section 6.4.3.1 of the xHCI spec
 */
struct xhci_trb_nop {
    uint8_t cycle  : 1;
    uint16_t reserved : 1;
    uint8_t type : 6;
    uint16_t reserved1;
    uint32_t reserved2[3];
};

#endif  /* !_USB_XHCIVAR_H_ */
