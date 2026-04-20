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

#ifndef _ACPI_ACPI_H_
#define _ACPI_ACPI_H_ 1

/*
 * Initialize ACPI, panics on failure
 */
void acpi_init(void);

/*
 * Query an ACPI table through its signature
 *
 * @signature: Signature to lookup
 *
 * Returns the virtual table base on success, otherwise
 * NULL on failure.
 */
void *acpi_query(const char *signature);

#endif  /* !_ACPI_ACPI_H_ */
