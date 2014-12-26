/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSCREENLINUXFB_QWS_H
#define QSCREENLINUXFB_QWS_H

#include <QtGui/qscreen_qws.h>

struct fb_cmap;
struct fb_var_screeninfo;
struct fb_fix_screeninfo;

#define QWS_OSD 1
#if QWS_OSD
//VPOST ===========================================================================================
// OSD
typedef enum {
  // All functions return -2 on "not open"
  OSD_Close=1,    // ()
  // Disables OSD and releases the buffers (??)
  // returns 0 on success

  OSD_Open,       // (cmd + color_format)
  // Opens OSD with color format
  // returns 0 on success

  OSD_Show,       // (cmd)
  // enables OSD mode
  // returns 0 on success

  OSD_Hide,       // (cmd)
  // disables OSD mode
  // returns 0 on success

  OSD_Clear,      // (cmd )
  // clear OSD buffer with color-key color
  // returns 0 on success

  OSD_Fill,      // (cmd +)
  // clear OSD buffer with assigned color
  // returns 0 on success

  OSD_FillBlock,      // (cmd+X-a
  // set OSD buffer with user color data (color data will be sent by "write()" function later
  // returns 0 on success

  OSD_SetTrans,   // (transparency{color})
  // Sets transparency color-key
  // returns 0 on success

  OSD_ClrTrans,   // (transparency{color})
  // Disable transparency color-key
  // returns 0 on success

} OSD_Command;

typedef struct osd_cmd_s {
        int cmd;
        int x0;
        int y0;
        int x0_size;
        int y0_size;
        int color;              // color_format, color_key
} osd_cmd_t;

#define DISPLAY_MODE_YCBYCR		5
#define DISPLAY_MODE_RGB565     	1
#define VIDEO_FORMAT_CHANGE		_IOW('v', 50, unsigned int)	//change video source format between RGB565 and YUV
#define IOCTL_GET_OSD_OFFSET    	_IOR('v', 60, unsigned int *)
#define IOCTL_LCD_ENABLE_INT		_IO('v', 28)
#define IOCTL_LCD_DISABLE_INT		_IO('v', 29)
#define IOCTL_LCD_BRIGHTNESS    	_IOW('v', 27, unsigned int)     //brightness control
#define VIDEO_DISPLAY_ON		_IOW('v', 24, unsigned int)     //display on
#define VIDEO_DISPLAY_OFF		_IOW('v', 25, unsigned int)     //display off
#define OSD_SEND_CMD           		_IOW('v', 160, unsigned int *)
#endif  // End of #if QWS_OSD

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

#ifndef QT_NO_QWS_LINUXFB

class QLinuxFb_Shared
{
public:
    volatile int lastop;
    volatile int optype;
    volatile int fifocount;   // Accel drivers only
    volatile int fifomax;
    volatile int forecol;     // Foreground colour caching
    volatile unsigned int buffer_offset;   // Destination
    volatile int linestep;
    volatile int cliptop;    // Clip rectangle
    volatile int clipleft;
    volatile int clipright;
    volatile int clipbottom;
    volatile unsigned int rop;

};

class QLinuxFbScreenPrivate;

class Q_GUI_EXPORT QLinuxFbScreen : public QScreen
{
public:
    explicit QLinuxFbScreen(int display_id);
    virtual ~QLinuxFbScreen();

    virtual bool initDevice();
    virtual bool connect(const QString &displaySpec);

    virtual bool useOffscreen();

    enum DriverTypes { GenericDriver, EInk8Track };

    virtual void disconnect();
    virtual void shutdownDevice();
    virtual void setMode(int,int,int);
    virtual void save();
    virtual void restore();
    virtual void blank(bool on);
    virtual void set(unsigned int,unsigned int,unsigned int,unsigned int);
    virtual uchar * cache(int);
    virtual void uncache(uchar *);
    virtual int sharedRamSize(void *);
    virtual void setDirty(const QRect&);

    QLinuxFb_Shared * shared;

protected:

#if QWS_OSD
    osd_cmd_t osd_block;
    int osd_offset;
    unsigned osd_colorkey;
#endif

    void deleteEntry(uchar *);

    bool canaccel;
    int dataoffset;
    int cacheStart;

    virtual void fixupScreenInfo(fb_fix_screeninfo &finfo, fb_var_screeninfo &vinfo);
    static void clearCache(QScreen *instance, int);

private:

    void delete_entry(int);
    void insert_entry(int,int,int);
    void setupOffScreen();
    void createPalette(fb_cmap &cmap, fb_var_screeninfo &vinfo, fb_fix_screeninfo &finfo);
    void setPixelFormat(struct fb_var_screeninfo);

    QLinuxFbScreenPrivate *d_ptr;
};

#endif // QT_NO_QWS_LINUXFB

QT_END_NAMESPACE

QT_END_HEADER

#endif // QSCREENLINUXFB_QWS_H
