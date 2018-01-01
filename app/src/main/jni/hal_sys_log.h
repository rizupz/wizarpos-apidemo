/*
 * Log.hpp
 *
 *  Created on: 2012-6-20
 *      Author: yaomaobiao
 */
#ifndef _HAL_SYS_LOG_H_
#define _HAL_SYS_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

void hal_sys_error(const char* pMessage, ...);
void hal_sys_warn(const char* pMessage, ...);
void hal_sys_info(const char* pMessage, ...);
void hal_sys_debug(const char* pMessage, ...);

void hal_sys_dump(const char * str, unsigned char * pBuf, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif /*_HAL_SYS_LOG_H_*/
