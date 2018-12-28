/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2005-2007 Dialogic Corporation. All Rights Reserved.
* The source code contained or described herein and all documents related
* to the source code ("Material") are owned by Dialogic Corporation or its
* suppliers or licensors. Title to the Material remains with Dialogic Corporation
* or its suppliers and licensors. The Material contains trade secrets and
* proprietary and confidential information of Dialogic or its suppliers and
* licensors. The Material is protected by worldwide copyright and trade secret
* laws and treaty provisions. No part of the Material may be used, copied,
* reproduced, modified, published, uploaded, posted, transmitted, distributed,
* or disclosed in any way without Dialogic's prior express written permission.
*
* No license under any patent, copyright, trade secret or other intellectual
* property right is granted to or conferred upon you by disclosure or delivery
* of the Materials, either expressly, by implication, inducement, estoppel or
* otherwise. Any license under such intellectual property rights must be
* express and approved by Dialogic in writing.
*
***********************************@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********/

#ifndef __DXUIO_H__
#define __DXUIO_H__

/*
 * User Defined I/O Functions for read()/write()/lseek()
 */

#if ( defined (__STDC__) || defined (__cplusplus) )

typedef struct dx_uio {
   int (* u_read)(int, char *, unsigned); /* User Defined read() */
   int (* u_write)(int, char *, unsigned);/* User Defined write() */
   long (* u_seek)(int, long, int);       /* User Defined lseek() */
} DX_UIO;

#else

typedef struct dx_uio {
   int (* u_read)();       /* User Defined replacement for read() */
   int (* u_write)();      /* User Defined replacement for write() */
   long (* u_seek)();       /* User Defined replacement for lseek() */
} DX_UIO;

#endif

/*
 * User Defined I/O Functions for direct play()/record()
 */
#if ( defined (__STDC__) || defined (__cplusplus) )

typedef struct dx_uio_direct {
   unsigned long uDirectFlags;
   int (*uPlayBufferAlloc)(int, char **, unsigned);
   void (*uPlayBufferFree)(int, char *);
   int (*uRecordBufferAlloc)(int, char **, unsigned);
   void (*uRecordBufferFree)(int, char *, unsigned);
} DX_UIO_DIRECT;

#else

typedef struct dx_uio_direct {
   unsigned long uDirectFlags;
   int (*uPlayBufferAlloc)();
   void (*uPlayBufferFree)();
   int (*uRecordBufferAlloc)();
   void (*uRecordBufferFree)();
} DX_UIO_DIRECT;

#endif

#endif	/* __DXUIO_H__	*/