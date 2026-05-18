/**************************************************************************//**
 * @file     strproc.h
 * @brief    The string processing API.
 * @version  V1.00
 * @date     2016-09-30
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef _STRPROC_H_
#define _STRPROC_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <stddef.h> /* for size_t */
#include <stdarg.h>

long _strtol(const char *nptr, char **endptr, int base);
long long _strtoll(const char *nptr, char **endptr, int base);
unsigned long _strtoul(const char *nptr, char **endptr, int base);
unsigned long long _strtoull(const char *nptr, char **endptr, int base);

#ifdef  __cplusplus
}
#endif

#endif

