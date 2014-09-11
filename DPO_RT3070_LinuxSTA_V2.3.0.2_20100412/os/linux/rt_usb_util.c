/*
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2007, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  * 
 * it under the terms of the GNU General Public License as published by  * 
 * the Free Software Foundation; either version 2 of the License, or     * 
 * (at your option) any later version.                                   * 
 *                                                                       * 
 * This program is distributed in the hope that it will be useful,       * 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        * 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         * 
 * GNU General Public License for more details.                          * 
 *                                                                       * 
 * You should have received a copy of the GNU General Public License     * 
 * along with this program; if not, write to the                         * 
 * Free Software Foundation, Inc.,                                       * 
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             * 
 *                                                                       * 
 *************************************************************************

	Module Name:
	rtusb_bulk.c

	Abstract:

	Revision History:
	Who			When		What
	--------	----------	----------------------------------------------
	Name		Date		Modification logs
	
*/

#include "rt_config.h"

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
 void dump_urb(struct urb* purb)
{
	printk("urb                  :0x%08lx\n", (unsigned long)purb);
	printk("\tdev                   :0x%08lx\n", (unsigned long)purb->dev);
	printk("\t\tdev->state          :0x%d\n", purb->dev->state);
	printk("\tpipe                  :0x%08x\n", purb->pipe);
	printk("\tstatus                :%d\n", purb->status);
	printk("\ttransfer_flags        :0x%08x\n", purb->transfer_flags);
	printk("\ttransfer_buffer       :0x%08lx\n", (unsigned long)purb->transfer_buffer);
	printk("\ttransfer_buffer_length:%d\n", purb->transfer_buffer_length);
	printk("\tactual_length         :%d\n", purb->actual_length);
	printk("\tsetup_packet          :0x%08lx\n", (unsigned long)purb->setup_packet);
	printk("\tstart_frame           :%d\n", purb->start_frame);
	printk("\tnumber_of_packets     :%d\n", purb->number_of_packets);
	printk("\tinterval              :%d\n", purb->interval);
	printk("\terror_count           :%d\n", purb->error_count);
	printk("\tcontext               :0x%08lx\n", (unsigned long)purb->context);
	printk("\tcomplete              :0x%08lx\n\n", (unsigned long)purb->complete);
}
#else
void dump_urb(struct urb* purb)
{
	return;
}
#endif // LINUX_VERSION_CODE //


#ifdef OS_ABL_SUPPORT
int rausb_register(struct usb_driver * new_driver)
{
	return usb_register(new_driver);
}
EXPORT_SYMBOL(rausb_register);


void rausb_deregister(struct usb_driver * driver)
{
	usb_deregister(driver);
}
EXPORT_SYMBOL(rausb_deregister);


struct urb *rausb_alloc_urb(int iso_packets)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	return usb_alloc_urb(iso_packets, GFP_ATOMIC);
#else
	return usb_alloc_urb(iso_packets);
#endif // LINUX_VERSION_CODE //
}
EXPORT_SYMBOL(rausb_alloc_urb);


void rausb_free_urb(struct urb *urb)
{
	usb_free_urb(urb);
}
EXPORT_SYMBOL(rausb_free_urb);


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
void rausb_put_dev(struct usb_device *dev)
{
	usb_put_dev(dev);
}
EXPORT_SYMBOL(rausb_put_dev);


struct usb_device *rausb_get_dev(struct usb_device *dev)
{
	return usb_get_dev(dev);
}
EXPORT_SYMBOL(rausb_get_dev);
#endif // LINUX_VERSION_CODE //


int rausb_submit_urb(struct urb *urb)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	return usb_submit_urb(urb, GFP_ATOMIC);
#else
	return usb_submit_urb(urb);
#endif // LINUX_VERSION_CODE //
}
EXPORT_SYMBOL(rausb_submit_urb);


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
void *rausb_buffer_alloc(struct usb_device *dev,
							size_t size,
							gfp_t mem_flags,
							dma_addr_t *dma)
{
	return usb_buffer_alloc(dev, size, mem_flags, dma);
}
EXPORT_SYMBOL(rausb_buffer_alloc);


void rausb_buffer_free(struct usb_device *dev,
							size_t size,
							void *addr,
							dma_addr_t dma)
{
	usb_buffer_free(dev, size, addr, dma);
}
EXPORT_SYMBOL(rausb_buffer_free);
#endif // LINUX_VERSION_CODE //


int rausb_control_msg(struct usb_device *dev,
						unsigned int pipe,
						__u8 request,
						__u8 requesttype,
						__u16 value,
						__u16 index,
						void *data,
						__u16 size,
						int timeout)
{
	return usb_control_msg(dev, pipe, request, requesttype, value, index,
							data, size, timeout);
}
EXPORT_SYMBOL(rausb_control_msg);


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,7)
void rausb_kill_urb(struct urb *urb)
{
	usb_kill_urb(urb);
}
EXPORT_SYMBOL(rausb_kill_urb);
#endif // LINUX_VERSION_CODE //

#endif // OS_ABL_SUPPORT //

/* End of rt_usb_util.c */
