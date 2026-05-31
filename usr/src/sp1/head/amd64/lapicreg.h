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

#ifndef _MACHINE_LAPICREG_H_
#define _MACHINE_LAPICREG_H_ 1

#include <sys/param.h>

/* LAPIC register offsets */
#define LAPIC_ID            0x0020U     /* ID Register */
#define LAPIC_VERSION       0x0030U     /* Version Register */
#define LAPIC_TPR           0x0080U     /* Task Priority Register */
#define LAPIC_APR           0x0090U     /* Arbitration Priority Register */
#define LAPIC_PPR           0x00A0U     /* Processor Priority Register */
#define LAPIC_EOI           0x00B0U     /* End Of Interrupt Register */
#define LAPIC_RRD           0x00C0U     /* Remote Read Register */
#define LAPIC_LDR           0x00D0U     /* Logical Destination Register */
#define LAPIC_DFR           0x00E0U     /* Destination Format Register */
#define LAPIC_SVR           0x00F0U     /* Spurious Vector Register */
#define LAPIC_ISR           0x0100U     /* In service register (max=0x0220) */
#define LAPIC_TMR           0x0180U     /* Trigger Mode Register (max=0x0220) */
#define LAPIC_IRR           0x0200U     /* Interrupt Request Register (max=0x0270) */
#define LAPIC_ERR           0x0280U     /* Error Status Register */
#define LAPIC_ICRLO         0x0300U     /* Interrupt Command Low Register */
#define LAPIC_ICRHI         0x0310U     /* Interrupt Command High Register */
#define LAPIC_LVT_TMR       0x0320U     /* LVT Timer Register */
#define LAPIC_DCR           0x03E0U     /* Divide Configuration Register (for timer) */
#define LAPIC_INIT_CNT      0x0380U     /* Initial Count Register (for timer) */
#define LAPIC_CUR_CNT       0x0390U     /* Current Count Register (for timer) */

/*
 * The x2APIC register space is accessed via
 * RDMSR/WRMSR instructions. The below defines
 * the base MSR address for the register space.
 */
#define x2APIC_MSR_BASE 0x00000800

/*
 * To hardware enable, OR the value of the IA32_APIC_BASE
 * MSR with LAPIC_HW_ENABLE and rewrite it.
 *
 * To software enable, OR the value of the SVR with
 * LAPIC_SW_ENABLE and rewrite it.
 *
 * LAPIC_SW_ENABLE has the low 8 bits set as some hardware
 * requires the spurious vector to be hardwired to 1s so
 * we'll go with that to be safe.
 */
#define LAPIC_HW_ENABLE     BIT(11)
#define LAPIC_SW_ENABLE     (BIT(8) | 0xFF)
#define x2APIC_ENABLE_SHIFT 10

/* LVT bits */
#define LAPIC_LVT_MASK            BIT(16)
#define LVT_TMR_ONESHOT           0x00
#define LVT_TMR_PERIODIC          0x01
#define LVT_TMR_TSC_DEADLINE      0x02

#endif  /* !_MACHINE_LAPICREG_H_ */
