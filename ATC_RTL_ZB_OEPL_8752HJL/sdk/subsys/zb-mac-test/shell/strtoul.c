/**************************************************************************//**
 * @file     strtoul.c
 * @brief    Implement the string to unsigned long covnersion function.
 * @version  V1.00
 * @date     2016-05-30
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2016 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 ******************************************************************************/
#include "basic_types.h"
#include "ctype.h"
//#include "section_config.h"
#if defined ( __GNUC__ )
#include <limits.h>
#else
#include "__limits.h"
#endif

/**
 *  @brief To conver a ASCII string to a unsigned long value.
 *
 *  @param nptr The start of the string buffer.
 *  @param endptr A pointer to the end of the parsed string will be placed here
 *  @param base The carry bits of the converted integer, Hex or Dec.
 *
 *  @returns The converted value.
 */
//INFRA_ROM_TEXT_SECTION
unsigned long _strtoul(const char *nptr, char **endptr, int base)
{
    register const unsigned char *s = (const unsigned char *)nptr;
    register unsigned long acc;
    register int c;
    register unsigned long cutoff;
    register int neg = 0, any, cutlim;

    /*
     * See strtol for comments as to the logic used.
     */
    do
    {
        c = *s++;
    }
    while (isspace(c));
    if (c == '-')
    {
        neg = 1;
        c = *s++;
    }
    else if (c == '+')
    {
        c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X'))
    {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
    {
        base = c == '0' ? 8 : 10;
    }
    cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
    cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
    for (acc = 0, any = 0;; c = *s++)
    {
        if (isdigit(c))
        {
            c -= '0';
        }
        else if (isalpha(c))
        {
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        }
        else
        {
            break;
        }
        if (c >= base)
        {
            break;
        }
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
        {
            any = -1;
        }
        else
        {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0)
    {
        acc = ULONG_MAX;
//      rptr->_errno = ERANGE;
    }
    else if (neg)
    {
        acc = -acc;
    }
    if (endptr != 0)
    {
        *endptr = (char *)(any ? (char *)s - 1 : nptr);
    }
    return (acc);
}


