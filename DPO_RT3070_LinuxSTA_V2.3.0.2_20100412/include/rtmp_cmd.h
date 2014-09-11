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

#ifndef __RTMP_CMD_H__
#define __RTMP_CMD_H__

#include "rtmp_type.h"


typedef struct _CmdQElmt {
	UINT				command;
	PVOID				buffer;
	ULONG				bufferlength;
	BOOLEAN				CmdFromNdis;
	BOOLEAN				SetOperation;
	struct _CmdQElmt	*next;
} CmdQElmt, *PCmdQElmt;

typedef struct _CmdQ {
	UINT				size;
	CmdQElmt			*head;
	CmdQElmt			*tail;
	UINT32				CmdQState;
} CmdQ, *PCmdQ;


#define EnqueueCmd(cmdq, cmdqelmt)		\
{										\
	if (cmdq->size == 0)				\
		cmdq->head = cmdqelmt;			\
	else								\
		cmdq->tail->next = cmdqelmt;	\
	cmdq->tail = cmdqelmt;				\
	cmdqelmt->next = NULL;				\
	cmdq->size++;						\
}

#define NDIS_OID	UINT

#ifdef KTHREAD_SUPPORT
#define RTCMDUp(pAd) \
	do{	\
		RTMP_OS_TASK	*_pTask = &((pAd)->cmdQTask);	\
		{ \
			_pTask->kthread_running = TRUE; \
	        wake_up(&_pTask->kthread_q); \
		} \
	}while(0)
#else
#define RTCMDUp(pAd)	                \
	do{									    \
		RTMP_OS_TASK	*_pTask = &((pAd)->cmdQTask);	\
		CHECK_PID_LEGALITY(_pTask->taskPID)	    \
		{\
			RTMP_SEM_EVENT_UP(&(_pTask->taskSema)); \
		}\
	}while(0)
#endif

#ifdef CONFIG_STA_SUPPORT
char * rtstrchr(const char * s, int c);

int rt_ioctl_setparam(struct net_device *dev, struct iw_request_info *info,
			 void *w, char *extra);

int rt_ioctl_siwscan(struct net_device *dev,
			struct iw_request_info *info,
			union iwreq_data *wreq, char *extra);
#endif // CONFIG_STA_SUPPORT //

void  getRate(HTTRANSMIT_SETTING HTSetting, ULONG* fLastTxRxRate);

#endif // __RTMP_CMD_H__ //
