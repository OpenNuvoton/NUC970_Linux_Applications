/* linux/arch/arm/mach-nuc970/include/mach/nuc970-etimer.h
 *
 * Copyright (c) 2014 Nuvoton technology corporation
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _NUC970_ETIMER_H_
#define _NUC970_ETIMER_H_

#include <linux/types.h>
#include <linux/ioctl.h>



#define ETMR_IOC_MAGIC		'e'
#define ETMR_IOC_MAXNR		3

#define ETMR_IOC_STOP			_IO(ETMR_IOC_MAGIC, 0)			/* Stop etimer */
#define ETMR_IOC_TOGGLE			_IOW(ETMR_IOC_MAGIC, 1, unsigned int *) /* Start etimer in toggle mode */
#define ETMR_IOC_FREE_COUNTING		_IOW(ETMR_IOC_MAGIC, 2, unsigned int *) /* Start etimer in free counting mode */
#define ETMR_IOC_TRIGGER_COUNTING	_IOW(ETMR_IOC_MAGIC, 3, unsigned int *) /* Start etimer in trigger counting mode */

// Valid parameters for capture mode ioctls
#define ETMR_CAP_EDGE_FF	0x00000  /* Capture start/stop trigger by falling edges */
#define ETMR_CAP_EDGE_RR	0x40000  /* Capture start/stop trigger by rising edges */
#define ETMR_CAP_EDGE_FR	0x80000  /* Capture start trigger by falling edge, stop trigger by rising edge */
#define ETMR_CAP_EDGE_RF	0xC0000  /* Capture start trigger by rising edge, stop trigger by falling edge */


#endif
