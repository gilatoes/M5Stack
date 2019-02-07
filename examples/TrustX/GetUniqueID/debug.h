/* This source code is forked from Infineon GitHub, frozen and modified for training purposes. 
 * Therefore, it is neither updated regularly nor qualify for production. 
 * Please refer to official Infineon GitHub https://github.com/Infineon/arduino-optiga-trust-x 
 * for the updated and latest release. 
 */
/**
 * MIT License
 *
 * Copyright (c) 2019 Infineon Technologies AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE
 *
 * Demonstrates use of the
 * Infineon Technologies AG OPTIGA™ Trust X Arduino library
 */
#ifndef FPRINT_H
#define FPRINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Arduino.h"

#define MAXCMD_LEN      255
#define HEXDUMP_COLS      16

#ifndef SUPPRESSHEXDUMP
#define SUPPRESSHEXDUMP   0
#endif
#define HEXDUMP(a, b)   (SUPPRESSHEXDUMP==0) ? __hexdump__(a,b) : (void) 0;

#define HEXARRAYDUMP(a, b)   __hexarraydump__(a,b)

#define HEXONLYDUMP(a, b)   __hexonlydump__(a,b)
#define BUF_SIZE 80
#include <stdarg.h>

int debug_print(char *str, ...)
{
  int i, count=0, j=0, flag=0;
  char temp[BUF_SIZE+1];
  for(i=0; str[i]!='\0';i++)  if(str[i]=='%')  count++;

  va_list argv;
  va_start(argv, count);
  for(i=0,j=0; str[i]!='\0';i++)
  {
    if(str[i]=='%')
    {
      temp[j] = '\0';
      Serial.print(temp);
      j=0;
      temp[0] = '\0';

      switch(str[++i])
      {
        case 'd': Serial.print(va_arg(argv, int));
                  break;
        case 'l': Serial.print(va_arg(argv, long));
                  break;
        case 'f': Serial.print(va_arg(argv, double));
                  break;
        case 'c': Serial.print((char)va_arg(argv, int));
                  break;
        case 's': Serial.print(va_arg(argv, char *));
                  break;
        case 'x': Serial.print(va_arg(argv, int),HEX);
                  break;
        default:  ;
      };
    }
    else
    {
      temp[j] = str[i];
      j = (j+1)%BUF_SIZE;
      if(j==0)
      {
        temp[BUF_SIZE] = '\0';
        Serial.print(temp);
        temp[0]='\0';
      }
    }
  };
  Serial.println();
  va_end(argv);
  return count + 1;
}

/**
 *
 * Printout data in a standard hex view
 *
 * @param[in] p_buf   Pointer to data which should be printed out.
 * @param[in] l_len   Length of a data
 *
 * @retval  None
 * @example
 0x000000: 2e 2f 68 65 78 64 75 6d ./hexdum
 0x000008: 70 00 53 53 48 5f 41 47 p.SSH_AG
 0x000010: 45 4e 54 5f             ENT_
 */
inline void __hexdump__(const void* p_buf, uint32_t l_len) {
  unsigned int i, j;
  static char str[MAXCMD_LEN];
  for (i = 0; i < l_len + ((l_len % HEXDUMP_COLS) ?
          ( HEXDUMP_COLS - l_len % HEXDUMP_COLS) : 0);
      i++) {
    /* print offset */
    if (i % HEXDUMP_COLS == 0) {
      sprintf(str, "0x%06x: ", i);
      Serial.print(str);
    }

    /* print hex data */
    if (i < l_len) {
      sprintf(str, "%02x ", 0xFF & ((char*) p_buf)[i]);
      Serial.print(str);
    } else /* end of block, just aligning for ASCII dump */
    {
      sprintf(str, "   ");
      Serial.print(str);
    }

    /* print ASCII dump */
    if (i % HEXDUMP_COLS == ( HEXDUMP_COLS - 1)) {
      for (j = i - ( HEXDUMP_COLS - 1); j <= i; j++) {
        if (j >= l_len) /* end of block, not really printing */
        {
          Serial.print(' ');
        } else if (isprint((int) ((char*) p_buf)[j])) /* printable char */
        {
          Serial.print(((char*) p_buf)[j]);
        } else /* other char */
        {
          Serial.print('.');
        }
      }
      Serial.print('\r');
      Serial.print('\n');
    }
  }
}

//Display the output. When in_len is 0, there is no data dump
static void output_result(uint32_t result, uint8_t* in, uint16_t in_len)
{
  if(result !=0){
    Serial.print("Error code:");
    Serial.println(result, HEX);
  }

  if(in_len!=0){
    HEXDUMP(in, in_len);
  }
}

/**
 *
 * Printout data for easy cut and paste into array structure
 *
 * @param[in] p_buf   Pointer to data which should be printed out.
 * @param[in] l_len   Length of a data
 *
 * @retval  None
 * @example  
2e 2f 68 65 78 64 75 6d 
70 00 53 53 48 5f 41 47 
45 4e 54 5f             
 */
inline void __hexarraydump__(const void* p_buf, uint32_t l_len) {
  unsigned int i, j;
  static char str[MAXCMD_LEN];
  for (i = 0; i < l_len + ((l_len % HEXDUMP_COLS) ?
          ( HEXDUMP_COLS - l_len % HEXDUMP_COLS) : 0);
      i++) {
    /* print offset */
    if (i % HEXDUMP_COLS == 0) {
      //sprintf(str, "0x%06x: ", i);
      //Serial.print(str);
    }

    /* print hex data */
    if (i < l_len) {
      sprintf(str, "0x%02x, ", 0xFF & ((char*) p_buf)[i]);
      Serial.print(str);
    } else /* end of block, just aligning for ASCII dump */
    {
      sprintf(str, "   ");
      Serial.print(str);
    }

    /* print ASCII dump */
    if (i % HEXDUMP_COLS == ( HEXDUMP_COLS - 1)) {
      for (j = i - ( HEXDUMP_COLS - 1); j <= i; j++) {
        if (j >= l_len) /* end of block, not really printing */
        {
          Serial.print(' ');
        } else if (isprint((int) ((char*) p_buf)[j])) /* printable char */
        {
          //Serial.print(((char*) p_buf)[j]);
        } else /* other char */
        {
          //Serial.print('.');
        }
      }
      Serial.print('\r');
      Serial.print('\n');
    }
    delay(10);
  }
}

/**
 *
 * Printout data for easy cut and paste into hex only format
 *
 * @param[in] p_buf   Pointer to data which should be printed out.
 * @param[in] l_len   Length of a data
 *
 * @retval  None
 * @example  
2e 2f 68 65 78 64 75 6d 
70 00 53 53 48 5f 41 47 
45 4e 54 5f             
 */
inline void __hexonlydump__(const void* p_buf, uint32_t l_len) {
  unsigned int i, j;
  static char str[MAXCMD_LEN];
  for (i = 0; i < l_len + ((l_len % HEXDUMP_COLS) ?
          ( HEXDUMP_COLS - l_len % HEXDUMP_COLS) : 0);
      i++) {
    /* print offset */
    if (i % HEXDUMP_COLS == 0) {
      //sprintf(str, "0x%06x: ", i);
      //Serial.print(str);
    }

    /* print hex data */
    if (i < l_len) {
      sprintf(str, "%02x ", 0xFF & ((char*) p_buf)[i]);
      Serial.print(str);
    } else /* end of block, just aligning for ASCII dump */
    {
      sprintf(str, "   ");
      Serial.print(str);
    }

    /* print ASCII dump */
    if (i % HEXDUMP_COLS == ( HEXDUMP_COLS - 1)) {
      for (j = i - ( HEXDUMP_COLS - 1); j <= i; j++) {
        if (j >= l_len) /* end of block, not really printing */
        {
          Serial.print(' ');
        } else if (isprint((int) ((char*) p_buf)[j])) /* printable char */
        {
          //Serial.print(((char*) p_buf)[j]);
        } else /* other char */
        {
          //Serial.print('.');
        }
      }
      Serial.print('\r');
      Serial.print('\n');
    }
    delay(10);
  }
}
#ifdef __cplusplus
}
#endif
#endif
