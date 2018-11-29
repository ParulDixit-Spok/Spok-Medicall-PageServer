/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2007 Dialogic Corporation. All Rights Reserved.
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
/**********************************************************************
*
*	C Header:		ipmedia_codecs.h
*	Description:	
*	%date_created:	Mon Oct 18 17:20:50 2004 %
*
**********************************************************************/
#ifndef _hsw_1_ipmedia_codecs_h_H
#define _hsw_1_ipmedia_codecs_h_H

typedef enum
{
   CODEC_DATA_T120                 = 0x01,   /* 1, T.120 data protocol chosen */
   CODEC_DATA_DSC_CC               = 0x02,   /* 2, DSM-cc data protocol chosen */
   CODEC_DATA_USRDATA              = 0x03,   /* 3, User data protocol chosen */
   CODEC_DATA_T84                  = 0x04,   /* 4, T.84 data protocol chosen */
   CODEC_DATA_T434                 = 0x05,   /* 5, T.434 data protocol chosen */
   CODEC_DATA_H224                 = 0x06,   /* 6, H.224 data protocol chosen */
   CODEC_DATA_NLPD                 = 0x07,   /* 7, NLPD data protocol chosen */
   CODEC_DATA_DSVDCONTROL          = 0x08,   /* 8, DSVD control data protocol chosen */
   CODEC_DATA_H222                 = 0x09,   /* 9, H.222 data protocol chosen */
   CODEC_DATA_T30FAX               = 0xa,    /* 10, T.30 Fax protocol chosen */
   CODEC_DATA_T140                 = 0xb,    /* 11, T.140 data protocol chosen */
   CODEC_DATA_T38FAX               = 0xc,    /* 12, T.38 Fax protocol chosen */
   CODEC_DATA_CCITTV110            = 0xd,    /* 13, CCITT V.110 standard */
   CODEC_DATA_CCITTV120            = 0xe,    /* 14, CCITT V.120 standard */
   CODEC_DATA_CCITTX31             = 0xf,    /* 15, CCITT X.31 standard */
   CODEC_DATA_NONSTANDARD          = 0x10,   /* 16, Non standard data capability */
   CODEC_AUDIO_NONSTANDARD         = 0x64,   /* 100, Non standard audio codec chosen */
   CODEC_AUDIO_G711ALAW64K         = 0x65,   /* 101, G.711 audio, A-law, 64k */
   CODEC_AUDIO_G711ALAW56K         = 0x66,   /* 102, G.711 audio, A-law, 56k */
   CODEC_AUDIO_G711ULAW64K         = 0x67,   /* 103, G.711 audio, U-law, 64k */
   CODEC_AUDIO_G711ULAW56K         = 0x68,   /* 104, G.711 audio, U-law, 56k */
   CODEC_AUDIO_G721ADPCM           = 0x69,   /* 105, ADPCM */
   CODEC_AUDIO_G722_48K            = 0x6a,   /* 106, G.722 audio 48k */
   CODEC_AUDIO_G722_56K            = 0x6b,   /* 107, G.722 audio 56k */
   CODEC_AUDIO_G722_64K            = 0x6c,   /* 108, G.722 audio 64k */
   CODEC_AUDIO_G7231_5_3K          = 0x6d,   /* 109, G.723.1 at 5.3 kbit/s */
   CODEC_AUDIO_G7231_6_3K          = 0x6e,   /* 110, G.723.1 at 6.3 kbit/s */
   CODEC_AUDIO_G7231               = CODEC_AUDIO_G7231_6_3K, /* 110: G.723.1 at received bit rate */
   CODEC_AUDIO_G726_16K            = 0x6f,   /* 111, G.726 at 16 kbit/s. Mapped to nonstandard CODEC in H.245 as G.726.is
                                                not yet specified in H.245  */
   CODEC_AUDIO_G726_24K            = 0x70,   /* 112, G.726 at 24 kbit/s. Mapped to nonstandard CODEC in H.245 as G.726.is 
                                                not yet specified in H.245  */
   CODEC_AUDIO_G726_32K            = 0x71,   /* 113, G.726 at 32 kbit/s. Mapped to nonstandard CODEC in H.245 as G.726.is 
                                                not yet specified in H.245  */
   CODEC_AUDIO_G726_40K            = 0x72,   /* 114, G.726 at 40 kbit/s. Mapped to nonstandard CODEC in H.245 as G.726.is 
                                                not yet specified in H.245  */
   CODEC_AUDIO_G728                = 0x73,   /* 115, G.728 audio at 16 kbit/s */
   CODEC_AUDIO_G729                = 0x74,   /* 116, G.729 audio at 8 kbit/s */
   CODEC_AUDIO_G729ANNEXA          = 0x76,   /* 118, G.729AnnexA audio at 8 kbit/s */
   CODEC_AUDIO_G729WANNEXB         = 0x77,   /* 119, G.729 audio at 8 kbit/s with silence suppression as in Annex B */
   CODEC_AUDIO_G729ANNEXAWANNEXB   = 0x78,   /* 120, G.729AnnexA audio at 8 kbit/s with silence suppression as in Annex B */
   CODEC_AUDIO_G7231ANNEXCCAP      = 0x79,   /* 121, G.723.1 with Annex C */
   CODEC_AUDIO_GSMFULLRATE         = 0x7a,   /* 122, Full-rate speech transcoding (GSM 06.10) */
   CODEC_AUDIO_GSMHALFRATE         = 0x7b,   /* 123, Half-rate speech transcoding (GSM 06.20) */
   CODEC_AUDIO_GSMEFULLRATE        = 0x7c,   /* 124, Enhanced Full Rate (EFR) speech transoding (GSM 06.60) */
   CODEC_AUDIO_GSMADAPTIVEMULTIRATE= 0x7d,   /* 125, GSM 06.90 AdaptiveMultRate.  Mapped to nonstandard CODEC in H.245
                                                as GSM AMR not yet specified in H.245  */
   CODEC_AUDIO_IS11172             = 0x7e,   /* 126, is11172AudioCapability_chosen */
   CODEC_AUDIO_IS13818             = 0x7f,   /* 127, is13818AudioCapability_chosen */
   CODEC_AUDIO_IS127EVARIABLERATE  = 0x80,   /* 128, TIA/EIA standard IS-127 transcoding.  Mapped to nonstandard
                                                CODEC in H.245 as IS-127 EVRC is not yet specified in H.245.  */
 /*The following Coders are GSM adaptive multirate narrow band coders that are 
   not part of the ITU standard coders, they run at 20ms framesize*/
   CODEC_AUDIO_AMRNB_4_75k			  = 0x82,	/* 130 AMR Narrow Band variable rate is 4.75k*/
   CODEC_AUDIO_AMRNB_5_15k			  = 0x83,	/* 131 AMR Narrow Band variable rate is 5.15kk*/
   CODEC_AUDIO_AMRNB_5_9k			  = 0x84,	/* 132 AMR Narrow Band variable rate is 5.9k*/
   CODEC_AUDIO_AMRNB_6_7k			  = 0x85,	/* 133 AMR Narrow Band variable rate is 6.7k*/
   CODEC_AUDIO_AMRNB_7_4k			  = 0x86,	/* 134 AMR Narrow Band variable rate is 7.4k*/
   CODEC_AUDIO_AMRNB_7_95k			  = 0x87,	/* 135 AMR Narrow Band variable rate is 7.95k*/
   CODEC_AUDIO_AMRNB_10_2k			  = 0x88,	/* 136 AMR Narrow Band variable rate is 10.2k*/
   CODEC_AUDIO_AMRNB_12_2k			  = 0x89,	/* 137 AMR Narrow Band variable rate is 12.2k*/

   CODEC_VIDEO_NONSTANDARD         = 0xc8,   /* 200, Non standard video codec chosen */
   CODEC_VIDEO_H261                = 0xc9,   /* 201, H.261 video codec */
   CODEC_VIDEO_H262                = 0xca,   /* 202, H.261 video codec */
   CODEC_VIDEO_H263                = 0xcb,   /* 203, H.263 video codec */
   CODEC_VIDEO_IS11172             = 0xcc,   /* 204, IS11172 video codec */
	CODEC_VIDEO_H263_1998           = 0xcd,   /* 205, H.263-1998 video codec */	
   CODEC_VIDEO_MP4V_ES				  = 0xce,   /* 206, MPEG4-ES video codec*/

   CODEC_OTHER_TXENCRYPTION        = 0x12c,  /* 300, Transmit enxryption capability */
   CODEC_OTHER_RXENCRYPTION        = 0x12d,  /* 301, Receive enxryption capability */
   CODEC_OTHER_CONFERENCE          = 0x12e,  /* 302, Conference capability */
   CODEC_OTHER_H235SECURITY        = 0x12f,  /* 303, H.235 security capability */
   CODEC_OTHER_CLIENTUSERINPUT     = 0x130,  /* 304, Client user input, used for DTMF tones */
   CODEC_OTHER_MUXNONSTANDARD      = 0x131,  /* 305, Non standard Mux capability */
   CODEC_OTHER_MUXH222             = 0x132,  /* 306, H.222 Mux capability */
   CODEC_OTHER_MUXH223             = 0x133,  /* 307, H.223 Mux capability */
   CODEC_OTHER_MUXVGMUX            = 0x134,  /* 308, VG Mux capability */
   CODEC_OTHER_MUXH2250            = 0x135,  /* 309, H.225.0 Mux capability */
   CODEC_OTHER_MUXH223ANNEXA       = 0x136,  /* 310, H.223 Annex A Mux capability */
   CODEC_OTHER_UNKNOWN             = 0x137,  /* 311, unknown capability */
   CODEC_OTHER_NOCHANGE            = 0x138,  /* 312, use previous capability */
   CODEC_OTHER_DONTCARE            = 0x139,  /* 313, use any capability */
   CODEC_OTHER_NONSTANDARD         = 0x13a  /* 314, Non standard capability */
}eIPMEDIA_CODEC_TYPE;

#endif /*_hsw_1_ipmedia_codecs_h_H*/
