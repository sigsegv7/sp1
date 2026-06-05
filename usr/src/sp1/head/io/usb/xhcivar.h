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

/*
 * Represents a xHCI host controller
 *
 * @max_slots: Maximum device slots
 * @max_ports: Maximum addressable ports
 */
struct xhci_hc {
    uint8_t max_slots;
    uint8_t max_ports;
};

#endif  /* !_USB_XHCIVAR_H_ */
