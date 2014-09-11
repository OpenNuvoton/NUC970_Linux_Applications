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
 *************************************************************************/

#include "rt_config.h"


// Following information will be show when you run 'modinfo'
// *** If you have a solution for the bug in current version of driver, please mail to me.
// Otherwise post to forum in ralinktech's web site(www.ralinktech.com) and let all users help you. ***
MODULE_AUTHOR("Paul Lin <paul_lin@ralinktech.com>");
MODULE_DESCRIPTION("RT2870 Wireless Lan Linux Driver");
#ifdef CONFIG_STA_SUPPORT
#ifdef MODULE_VERSION
MODULE_VERSION(STA_DRIVER_VERSION);
#endif
#endif // CONFIG_STA_SUPPORT //


extern USB_DEVICE_ID rtusb_dev_id[];
extern INT const rtusb_usb_id_len;

static void rt2870_disconnect(
	IN struct usb_device *dev, 
	IN PRTMP_ADAPTER pAd);

static int rt2870_probe(
	IN struct usb_interface *intf,
	IN struct usb_device *usb_dev,
	IN const USB_DEVICE_ID *dev_id,
	IN RTMP_ADAPTER **ppAd);

#ifndef PF_NOFREEZE
#define PF_NOFREEZE  0
#endif


//extern int rt28xx_close(IN struct net_device *net_dev);
//extern int rt28xx_open(struct net_device *net_dev);

static BOOLEAN USBDevConfigInit(
	IN struct usb_device 	*dev,
	IN struct usb_interface *intf, 
	IN RTMP_ADAPTER *pAd);
	

VOID RT28XXVendorSpecificCheck(
	IN struct usb_device 	*dev,
	IN RTMP_ADAPTER 	*pAd)
{
    	if (dev->descriptor.idVendor == 0x0DB0)
	{
		if ((dev->descriptor.idProduct == 0x871C) || (dev->descriptor.idProduct == 0x822C))
		{
			RTMP_SET_MORE_FLAG(pAd, (fRTMP_ADAPTER_DISABLE_DOT_11N | fRTMP_ADAPTER_WSC_PBC_PIN0));
		}
		if ((dev->descriptor.idProduct == 0x871A) || (dev->descriptor.idProduct == 0x822A))
		{
			RTMP_SET_MORE_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DOT_11N);
		}
		if ((dev->descriptor.idProduct == 0x871B) || (dev->descriptor.idProduct == 0x822B))
		{
			RTMP_SET_MORE_FLAG(pAd, fRTMP_ADAPTER_WSC_PBC_PIN0);
		}
	}

    	if (dev->descriptor.idVendor == 0x07D1)
    	{
		if (dev->descriptor.idProduct == 0x3C0F)
			RTMP_SET_MORE_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DOT_11N);
    	}
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)

/**************************************************************************/
/**************************************************************************/
//tested for kernel 2.4 series
/**************************************************************************/
/**************************************************************************/
static void *rtusb_probe(struct usb_device *dev, UINT interface,
						const USB_DEVICE_ID *id_table);
static void rtusb_disconnect(struct usb_device *dev, void *ptr);

struct usb_driver rtusb_driver = {
		name:"rt2870",
		probe:rtusb_probe,
		disconnect:rtusb_disconnect,
		id_table:rtusb_dev_id,
	};


static BOOLEAN USBDevConfigInit(
	IN struct usb_device 	*dev,
	IN struct usb_interface *intf, 
	IN RTMP_ADAPTER *pAd)
{
	struct usb_interface_descriptor *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	ULONG BulkOutIdx;
	UINT32 i;


	iface_desc = &intf->altsetting[0];

	/* get # of enpoints */
	pAd->NumberOfPipes = iface_desc->bNumEndpoints;
	DBGPRINT(RT_DEBUG_TRACE, ("NumEndpoints=%d\n", iface_desc->bNumEndpoints));		 

	/* Configure Pipes */
	endpoint = &iface_desc->endpoint[0];
	BulkOutIdx = 0;

	for(i=0; i<pAd->NumberOfPipes; i++)
	{
		if ((endpoint[i].bmAttributes == USB_ENDPOINT_XFER_BULK) && 
			((endpoint[i].bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN))
		{
			pAd->BulkInEpAddr = endpoint[i].bEndpointAddress;
			pAd->BulkInMaxPacketSize = endpoint[i].wMaxPacketSize;

			DBGPRINT_RAW(RT_DEBUG_TRACE, ("BULK IN MaximumPacketSize = %d\n", pAd->BulkInMaxPacketSize));
			DBGPRINT_RAW(RT_DEBUG_TRACE, ("EP address = 0x%2x  \n", endpoint[i].bEndpointAddress));
		}
		else if ((endpoint[i].bmAttributes == USB_ENDPOINT_XFER_BULK) && 
				((endpoint[i].bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT))
		{
			// There are 6 bulk out EP. EP6 highest priority.
			// EP1-4 is EDCA.  EP5 is HCCA.
			pAd->BulkOutEpAddr[BulkOutIdx++] = endpoint[i].bEndpointAddress;
			pAd->BulkOutMaxPacketSize = endpoint[i].wMaxPacketSize;

			DBGPRINT_RAW(RT_DEBUG_TRACE, ("BULK OUT MaximumPacketSize = %d\n", pAd->BulkOutMaxPacketSize));
			DBGPRINT_RAW(RT_DEBUG_TRACE, ("EP address = 0x%2x  \n", endpoint[i].bEndpointAddress));
		}
	}

	if (!(pAd->BulkInEpAddr && pAd->BulkOutEpAddr[0])) 
	{
		printk("Could not find both bulk-in and bulk-out endpoints\n");
		return FALSE;
	}

	pAd->config = dev->config;
	RT28XXVendorSpecificCheck(dev, pAd);

	return TRUE;
	
}

static void *rtusb_probe(struct usb_device *dev, UINT interface,
						const USB_DEVICE_ID *id)
{
	struct usb_interface *intf;
	RTMP_ADAPTER *pAd;
	int rv;


	/* get the active interface descriptor */
	intf = &dev->actconfig->interface[interface];

	// call generic probe procedure.	
	rv = rt2870_probe(intf, dev, id, &pAd);
	if (rv != 0)
		pAd = NULL;
	
	return (void *)pAd;
}

//Disconnect function is called within exit routine
static void rtusb_disconnect(struct usb_device *dev, void *ptr)
{
	rt2870_disconnect(dev, ((PRTMP_ADAPTER)ptr));
}


#else	// else if we are kernel 2.6 series


/**************************************************************************/
/**************************************************************************/
//tested for kernel 2.6series
/**************************************************************************/
/**************************************************************************/

#ifdef CONFIG_PM

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,10)
#define pm_message_t u32
#endif

static int rt2870_suspend(struct usb_interface *intf, pm_message_t state);
static int rt2870_resume(struct usb_interface *intf);
#endif // CONFIG_PM //

static int rtusb_probe (struct usb_interface *intf,
						const USB_DEVICE_ID *id);
static void rtusb_disconnect(struct usb_interface *intf);

static BOOLEAN USBDevConfigInit(
	IN struct usb_device 	*dev,
	IN struct usb_interface *intf, 
	IN RTMP_ADAPTER 	*pAd)
{
	struct usb_host_interface *iface_desc;
	ULONG BulkOutIdx;
	UINT32 i;


	/* get the active interface descriptor */
	iface_desc = intf->cur_altsetting;

	/* get # of enpoints  */
	pAd->NumberOfPipes = iface_desc->desc.bNumEndpoints;
	DBGPRINT(RT_DEBUG_TRACE, ("NumEndpoints=%d\n", iface_desc->desc.bNumEndpoints));		  

	/* Configure Pipes */
	BulkOutIdx = 0;

	for(i=0; i<pAd->NumberOfPipes; i++)
	{ 
		if ((iface_desc->endpoint[i].desc.bmAttributes == 
				USB_ENDPOINT_XFER_BULK) && 
			((iface_desc->endpoint[i].desc.bEndpointAddress &
				USB_ENDPOINT_DIR_MASK) == USB_DIR_IN))
		{
			pAd->BulkInEpAddr = iface_desc->endpoint[i].desc.bEndpointAddress;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
			pAd->BulkInMaxPacketSize = le2cpu16(iface_desc->endpoint[i].desc.wMaxPacketSize);
#else
			pAd->BulkInMaxPacketSize = iface_desc->endpoint[i].desc.wMaxPacketSize;
#endif // LINUX_VERSION_CODE //

			DBGPRINT_RAW(RT_DEBUG_TRACE, ("BULK IN MaxPacketSize = %d\n", pAd->BulkInMaxPacketSize));
			DBGPRINT_RAW(RT_DEBUG_TRACE, ("EP address = 0x%2x\n", iface_desc->endpoint[i].desc.bEndpointAddress));
		}
		else if ((iface_desc->endpoint[i].desc.bmAttributes ==
					USB_ENDPOINT_XFER_BULK) && 
				((iface_desc->endpoint[i].desc.bEndpointAddress &
					USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT))
		{
			// there are 6 bulk out EP. EP6 highest priority.
			// EP1-4 is EDCA.  EP5 is HCCA.
			pAd->BulkOutEpAddr[BulkOutIdx++] = iface_desc->endpoint[i].desc.bEndpointAddress;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
			pAd->BulkOutMaxPacketSize = le2cpu16(iface_desc->endpoint[i].desc.wMaxPacketSize);
#else
			pAd->BulkOutMaxPacketSize = iface_desc->endpoint[i].desc.wMaxPacketSize;
#endif

			DBGPRINT_RAW(RT_DEBUG_TRACE, ("BULK OUT MaxPacketSize = %d\n", pAd->BulkOutMaxPacketSize));
			DBGPRINT_RAW(RT_DEBUG_TRACE, ("EP address = 0x%2x  \n", iface_desc->endpoint[i].desc.bEndpointAddress));
		}
	}

	if (!(pAd->BulkInEpAddr && pAd->BulkOutEpAddr[0])) 
	{
		printk("%s: Could not find both bulk-in and bulk-out endpoints\n", __FUNCTION__);
		return FALSE;
	}

	pAd->config = &dev->config->desc;
	usb_set_intfdata(intf, pAd);
	RT28XXVendorSpecificCheck(dev, pAd);    
	
	return TRUE;
	
}



static int rtusb_probe (struct usb_interface *intf,
						const USB_DEVICE_ID *id)
{	
	RTMP_ADAPTER *pAd;
	struct usb_device *dev;
	int rv;

	dev = interface_to_usbdev(intf);
	dev = usb_get_dev(dev);
	
	rv = rt2870_probe(intf, dev, id, &pAd);
	if (rv != 0)
		usb_put_dev(dev);
	
	return rv;
}


static void rtusb_disconnect(struct usb_interface *intf)
{
	struct usb_device   *dev = interface_to_usbdev(intf);
	PRTMP_ADAPTER       pAd;


	pAd = usb_get_intfdata(intf);
	usb_set_intfdata(intf, NULL);	

	rt2870_disconnect(dev, pAd);
}


struct usb_driver rtusb_driver = {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15)
	.owner = THIS_MODULE,
#endif	
	.name="rt2870",
	.probe=rtusb_probe,
	.disconnect=rtusb_disconnect,
	.id_table=rtusb_dev_id,

#ifdef CONFIG_PM
	suspend:	rt2870_suspend,
	resume:		rt2870_resume,
#endif
	};

#ifdef CONFIG_PM

VOID RT2870RejectPendingPackets(
	IN	PRTMP_ADAPTER	pAd)
{
	// clear PS packets
	// clear TxSw packets
}

static int rt2870_suspend(
	struct usb_interface *intf,
	pm_message_t state)
{
	struct net_device *net_dev;
	PRTMP_ADAPTER pAd = usb_get_intfdata(intf);


	DBGPRINT(RT_DEBUG_TRACE, ("===> rt2870_suspend()\n"));
	net_dev = pAd->net_dev;
	netif_device_detach(net_dev);

	pAd->PM_FlgSuspend = 1;
	if (netif_running(net_dev)) {
		RTUSBCancelPendingBulkInIRP(pAd);
		RTUSBCancelPendingBulkOutIRP(pAd);
	}
	DBGPRINT(RT_DEBUG_TRACE, ("<=== rt2870_suspend()\n"));
	return 0;
}

static int rt2870_resume(
	struct usb_interface *intf)
{
	struct net_device *net_dev;
	PRTMP_ADAPTER pAd = usb_get_intfdata(intf);


	DBGPRINT(RT_DEBUG_TRACE, ("===> rt2870_resume()\n"));

	pAd->PM_FlgSuspend = 0;
	net_dev = pAd->net_dev;
	netif_device_attach(net_dev);
	netif_start_queue(net_dev);
	netif_carrier_on(net_dev);
	netif_wake_queue(net_dev);

	DBGPRINT(RT_DEBUG_TRACE, ("<=== rt2870_resume()\n"));
	return 0;
}
#endif // CONFIG_PM //
#endif // LINUX_VERSION_CODE //


// Init driver module
INT __init rtusb_init(void)
{
	printk("rtusb init --->\n");   
	return usb_register(&rtusb_driver);
}

// Deinit driver module
VOID __exit rtusb_exit(void)
{
	usb_deregister(&rtusb_driver);	
	printk("<--- rtusb exit\n");
}

module_init(rtusb_init);
module_exit(rtusb_exit);




/*---------------------------------------------------------------------	*/
/* function declarations												*/
/*---------------------------------------------------------------------	*/

/*
========================================================================
Routine Description:
    MLME kernel thread.

Arguments:
	*Context			the pAd, driver control block pointer

Return Value:
    0					close the thread

Note:
========================================================================
*/
INT MlmeThread(
	IN ULONG Context)
{
	RTMP_ADAPTER *pAd;
	RTMP_OS_TASK *pTask;
	int status;
	status = 0;

	pTask = (RTMP_OS_TASK *)Context;
	pAd = (PRTMP_ADAPTER)pTask->priv;
	
	RtmpOSTaskCustomize(pTask);

	while(!pTask->task_killed)
	{
#ifdef KTHREAD_SUPPORT
		RTMP_WAIT_EVENT_INTERRUPTIBLE(pAd, pTask);
#else
		RTMP_SEM_EVENT_WAIT(&(pTask->taskSema), status);
			
		/* unlock the device pointers */
		if (status != 0)
		{
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
			break;
		}	
#endif
			
		/* lock the device pointers , need to check if required*/
		//down(&(pAd->usbdev_semaphore));
		
		if (!pAd->PM_FlgSuspend)
			MlmeHandler(pAd);
	}

	/* notify the exit routine that we're actually exiting now 
	 *
	 * complete()/wait_for_completion() is similar to up()/down(),
	 * except that complete() is safe in the case where the structure
	 * is getting deleted in a parallel mode of execution (i.e. just
	 * after the down() -- that's necessary for the thread-shutdown
	 * case.
	 *
	 * complete_and_exit() goes even further than this -- it is safe in
	 * the case that the thread of the caller is going away (not just
	 * the structure) -- this is necessary for the module-remove case.
	 * This is important in preemption kernels, which transfer the flow
	 * of execution immediately upon a complete().
	 */
	DBGPRINT(RT_DEBUG_TRACE,( "<---%s\n",__FUNCTION__));
#ifndef KTHREAD_SUPPORT
	pTask->taskPID = THREAD_PID_INIT_VALUE;
	complete_and_exit (&pTask->taskComplete, 0);
#endif
	return 0;

}


/*
========================================================================
Routine Description:
    USB command kernel thread.

Arguments:
	*Context			the pAd, driver control block pointer

Return Value:
    0					close the thread

Note:
========================================================================
*/
INT RTUSBCmdThread(
	IN ULONG Context)
{
	RTMP_ADAPTER *pAd;
	RTMP_OS_TASK *pTask;
	int status;
	status = 0;

	pTask = (RTMP_OS_TASK *)Context;
	pAd = (PRTMP_ADAPTER)pTask->priv;
	
	RtmpOSTaskCustomize(pTask);

	NdisAcquireSpinLock(&pAd->CmdQLock);
	pAd->CmdQ.CmdQState = RTMP_TASK_STAT_RUNNING;
	NdisReleaseSpinLock(&pAd->CmdQLock);
	
	while (pAd && pAd->CmdQ.CmdQState == RTMP_TASK_STAT_RUNNING)
	{
#ifdef KTHREAD_SUPPORT
		RTMP_WAIT_EVENT_INTERRUPTIBLE(pAd, pTask);
#else
		/* lock the device pointers */
		RTMP_SEM_EVENT_WAIT(&(pTask->taskSema), status);

		if (status != 0)
		{
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
			break;
		}
#endif

		if (pAd->CmdQ.CmdQState == RTMP_TASK_STAT_STOPED)
			break;

		if (!pAd->PM_FlgSuspend)
			CMDHandler(pAd);
	}

	if (pAd && !pAd->PM_FlgSuspend)
	{	// Clear the CmdQElements.
		CmdQElmt	*pCmdQElmt = NULL;

		NdisAcquireSpinLock(&pAd->CmdQLock);
		pAd->CmdQ.CmdQState = RTMP_TASK_STAT_STOPED;
		while(pAd->CmdQ.size)
		{
			RTThreadDequeueCmd(&pAd->CmdQ, &pCmdQElmt);
			if (pCmdQElmt)
			{
				if (pCmdQElmt->CmdFromNdis == TRUE)
				{
					if (pCmdQElmt->buffer != NULL)
						os_free_mem(pAd, pCmdQElmt->buffer);
					os_free_mem(pAd, (PUCHAR)pCmdQElmt);
				}
				else
				{
					if ((pCmdQElmt->buffer != NULL) && (pCmdQElmt->bufferlength != 0))
						os_free_mem(pAd, pCmdQElmt->buffer);
					os_free_mem(pAd, (PUCHAR)pCmdQElmt);
				}
			}
		}

		NdisReleaseSpinLock(&pAd->CmdQLock);
	}
	/* notify the exit routine that we're actually exiting now 
	 *
	 * complete()/wait_for_completion() is similar to up()/down(),
	 * except that complete() is safe in the case where the structure
	 * is getting deleted in a parallel mode of execution (i.e. just
	 * after the down() -- that's necessary for the thread-shutdown
	 * case.
	 *
	 * complete_and_exit() goes even further than this -- it is safe in
	 * the case that the thread of the caller is going away (not just
	 * the structure) -- this is necessary for the module-remove case.
	 * This is important in preemption kernels, which transfer the flow
	 * of execution immediately upon a complete().
	 */
	DBGPRINT(RT_DEBUG_TRACE,( "<---RTUSBCmdThread\n"));

#ifndef KTHREAD_SUPPORT
	pTask->taskPID = THREAD_PID_INIT_VALUE;
	complete_and_exit (&pTask->taskComplete, 0);
#endif
	return 0;

}


/*
========================================================================
Routine Description:
    Release allocated resources.

Arguments:
    *dev				Point to the PCI or USB device
	pAd					driver control block pointer

Return Value:
    None

Note:
========================================================================
*/
static void rt2870_disconnect(struct usb_device *dev, PRTMP_ADAPTER pAd)
{
	DBGPRINT(RT_DEBUG_ERROR, ("rtusb_disconnect: unregister usbnet usb-%s-%s\n",
				dev->bus->bus_name, dev->devpath));
	if (!pAd)
	{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)	/* kernel 2.4 series */
		while(MOD_IN_USE > 0)
		{
			MOD_DEC_USE_COUNT;
		}
#else
		usb_put_dev(dev);
#endif // LINUX_VERSION_CODE //

		printk("rtusb_disconnect: pAd == NULL!\n");
		return;
	}
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);

	// for debug, wait to show some messages to /proc system
	udelay(1);


	RtmpPhyNetDevExit(pAd, pAd->net_dev);

	// FIXME: Shall we need following delay and flush the schedule??
	udelay(1);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)	/* kernel 2.4 series */
#else
	flush_scheduled_work();
#endif // LINUX_VERSION_CODE //
	udelay(1);

	// free the root net_device
	RtmpOSNetDevFree(pAd->net_dev);

#ifdef RT_CFG80211_SUPPORT
	CFG80211_UnRegister(pAd, pAd->net_dev);
#endif // RT_CFG80211_SUPPORT //

	RtmpRaDevCtrlExit(pAd);

	// release a use of the usb device structure
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)	/* kernel 2.4 series */
	while(MOD_IN_USE > 0)
	{
		MOD_DEC_USE_COUNT;
	}
#else
	usb_put_dev(dev);
#endif // LINUX_VERSION_CODE //
	udelay(1);

	DBGPRINT(RT_DEBUG_ERROR, (" RTUSB disconnect successfully\n"));
}


static int rt2870_probe(
	IN struct usb_interface *intf,
	IN struct usb_device *usb_dev,
	IN const USB_DEVICE_ID *dev_id,
	IN RTMP_ADAPTER **ppAd)
{
	struct  net_device		*net_dev = NULL;
	RTMP_ADAPTER       	*pAd = (RTMP_ADAPTER *) NULL;
	INT                 		status, rv;
	PVOID				handle;
	RTMP_OS_NETDEV_OP_HOOK	netDevHook;
	

	DBGPRINT(RT_DEBUG_TRACE, ("===>rt2870_probe()!\n"));
	
	// Check chipset vendor/product ID
	//if (RT28XXChipsetCheck(_dev_p) == FALSE)
	//	goto err_out;

//RtmpDevInit=============================================
	// Allocate RTMP_ADAPTER adapter structure
	handle = kmalloc(sizeof(struct os_cookie), GFP_KERNEL);
	if (handle == NULL)
	{
		printk("rt2870_probe(): Allocate memory for os handle failed!\n");
		return -ENOMEM;
	}
	((POS_COOKIE)handle)->pUsb_Dev = usb_dev;
	
	rv = RTMPAllocAdapterBlock(handle, &pAd);
	if (rv != NDIS_STATUS_SUCCESS) 
	{
		kfree(handle);
		goto err_out;
	}

//USBDevInit==============================================
	if (USBDevConfigInit(usb_dev, intf, pAd) == FALSE)
		goto err_out_free_radev;
	
	RtmpRaDevCtrlInit(pAd, RTMP_DEV_INF_USB);
	
//NetDevInit==============================================
	net_dev = RtmpPhyNetDevInit(pAd, &netDevHook);
	if (net_dev == NULL)
		goto err_out_free_radev;
	
	// Here are the net_device structure with usb specific parameters.
#ifdef NATIVE_WPA_SUPPLICANT_SUPPORT
	/* for supporting Network Manager.
	  * Set the sysfs physical device reference for the network logical device if set prior to registration will 
	  * cause a symlink during initialization.
	 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
	SET_NETDEV_DEV(net_dev, &(usb_dev->dev));
#endif
#endif // NATIVE_WPA_SUPPLICANT_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
    pAd->StaCfg.OriDevType = net_dev->type;
#endif // CONFIG_STA_SUPPORT //

//All done, it's time to register the net device to linux kernel.
	// Register this device
	status = RtmpOSNetDevAttach(net_dev, &netDevHook);
	if (status != 0)
		goto err_out_free_netdev;

#ifdef KTHREAD_SUPPORT
	init_waitqueue_head(&pAd->mlmeTask.kthread_q);
	init_waitqueue_head(&pAd->timerTask.kthread_q);
	init_waitqueue_head(&pAd->cmdQTask.kthread_q);
#endif	

#ifdef RT_CFG80211_SUPPORT
	pAd->pCfgDev = &(usb_dev->dev);
#endif // RT_CFG80211_SUPPORT //

	*ppAd = pAd;

#ifdef INF_AMAZON_PPA
	pAd->pDirectpathCb = (PPA_DIRECTPATH_CB *) kmalloc (sizeof(PPA_DIRECTPATH_CB), GFP_ATOMIC);
#endif // INF_AMAZON_PPA //

	DBGPRINT(RT_DEBUG_TRACE, ("<===rt2870_probe()!\n"));

	return 0;

	/* --------------------------- ERROR HANDLE --------------------------- */	
err_out_free_netdev:
	RtmpOSNetDevFree(net_dev);
	
err_out_free_radev:
	RTMPFreeAdapter(pAd);
	
err_out:
	*ppAd = NULL;

	return -1;
	
}

