/*
 * Egis Technology Inc. (aka. LighTuning) 0570 driver for libfprint
 * Copyright (C) 2021 Maxim Kolesnikov <kolesnikov@svyazcom.ru>
 * Copyright (C) 2021 Saeed/Ali Rk <saeed.ali.rahimi@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __EGIS0575_H

#define __EGIS0575_H 1

/*
 * Device data
 */

#define EGIS0575_CONF 1
#define EGIS0575_INTF 0

/*
 * Device endpoints
 */

#define EGIS0575_EPOUT 0x01 /* ( 1 | FPI_USB_ENDPOINT_OUT ) */
#define EGIS0575_EPIN 0x82  /* ( 4 | FPI_USB_ENDPOINT_IN ) */

/*
 * Initialization packets (7 bytes each)
 *
 * First 4 bytes are equivalent to string "EGIS", which must be just a company identificator
 * Other 3 bytes are not recognized yet and may be not important, as they are always the same

 * Answers for each packet contain 7 bytes again
 * First 4 bytes are reversed "EGIS", which is "SIGE", which is company ID again
 * Other 3 bytes are not recognized yet
 * But there is a pattern.
 * Sending last packet makes sensor return image
 */

#define EGIS0575_TIMEOUT 10000
#define EGIS0575_INIT_PKTSIZE 18
#define EGIS0575_REPEAT_PKTSIZE 9

#define EGIS0575_INIT_TOTAL (sizeof ((init_pkts)) / sizeof ((init_pkts[0])))

static unsigned char init_pkts[][EGIS0575_INIT_PKTSIZE] =
{
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x00, 0xfc },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x01, 0xfc },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x40, 0xfc },
  { 0x45, 0x47, 0x49, 0x53, 0x63, 0x09, 0x0b, 0x83, 0x24, 0x00, 0x44, 0x0f, 0x08, 0x20, 0x20, 0x01, 0x05, 0x12 },
  { 0x45, 0x47, 0x49, 0x53, 0x64, 0x26, 0x06, 0x06, 0x60, 0x06, 0x05, 0x2f, 0x06 },
  { 0x45, 0x47, 0x49, 0x53, 0x61, 0x23, 0x00 },
  { 0x45, 0x47, 0x49, 0x53, 0x61, 0x24, 0x33 },
  { 0x45, 0x47, 0x49, 0x53, 0x61, 0x21, 0x66 },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x00, 0x66 },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x01, 0x66 },
  { 0x45, 0x47, 0x49, 0x53, 0x63, 0x2c, 0x02, 0x00, 0x57 },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x2d, 0x02 },
  { 0x45, 0x47, 0x49, 0x53, 0x62, 0x67, 0x03 },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x0f, 0x03 },
  { 0x45, 0x47, 0x49, 0x53, 0x63, 0x2c, 0x02, 0x00, 0x013 },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x00, 0x02 },
  { 0x45, 0x47, 0x49, 0x53, 0x64, 0x14, 0xec }        /* image returned after this packet */
};

/*
 * After sending initial packets device returns image data (32512 bytes)
 * To ask device to send image data again, host needs to send nine additional packets
 * Further work is to repeatedly send four repeat packets and read image data
 */

#define EGIS0575_INPSIZE 32512

/* 5 image with captured in different time of size 114 * 57 = 6498
 * 5 * 6498 = 32490 plus 22 extra unrecognized char size data
 * Two continuous image in this 5 images may have time delay of less than 20ms
 */

#define EGIS0575_IMGSIZE 6498
#define EGIS0575_IMGWIDTH 114
#define EGIS0575_IMGHEIGHT 57

/* size of middle area that is used from each frame */
#define EGIS0575_RFMGHEIGHT 17
/* rows to ignore from top and bottom of the image*/
#define EGIS0575_RFMDIS (EGIS0575_IMGHEIGHT - EGIS0575_RFMGHEIGHT) / 2
#define EGIS0575_IMGCOUNT 5

/*
 * Image repeat request
 * First 4 bytes are the same as in initialization packets
 * Have no idea what the other 3 bytes mean
 */

#define EGIS0575_REPEAT_TOTAL (sizeof ((repeat_pkts)) / sizeof ((repeat_pkts[0])))

static unsigned char repeat_pkts[][EGIS0575_REPEAT_PKTSIZE] =
{
  { 0x45, 0x47, 0x49, 0x53, 0x61, 0x2d, 0x20 },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x00, 0x20 },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x01, 0x20 },
  { 0x45, 0x47, 0x49, 0x53, 0x63, 0x2c, 0x02, 0x00, 0x57 },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x2d, 0x02 },
  { 0x45, 0x47, 0x49, 0x53, 0x62, 0x67, 0x03 },
  { 0x45, 0x47, 0x49, 0x53, 0x63, 0x2c, 0x02, 0x00, 0x13 },
  { 0x45, 0x47, 0x49, 0x53, 0x60, 0x00, 0x02 },
  { 0x45, 0x47, 0x49, 0x53, 0x64, 0x14, 0xec }, /* image returned after this packet */
};

/*
 * This sensor is small so I decided to reduce bz3_threshold from
 * 40 to 10 to have more success to fail ratio
 * Bozorth3 Algorithm seems not fine at the end
 * foreget about security :))
 */

#define EGIS0575_BZ3_THRESHOLD 25 /* and even less What a joke */

#define EGIS0575_MIN_MEAN 20
#define EGIS0575_MARGIN 3

#define EGIS0575_RESIZE 2

#endif
