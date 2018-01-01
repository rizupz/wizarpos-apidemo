/*
 * msr_impl.h
 *
 *  Created on: 2015-9-11
 *      Author: zhangxudong
 */

#ifndef MSR_INTERFACE_H_
#define MSR_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*msr_notifier)(void* pUserData);

typedef int (*msr_open)(void);
typedef int (*msr_close)(void);
typedef int (*msr_register_notifier)(msr_notifier notifier, void* pUserData);
typedef int (*msr_unregister_notifier)();
typedef int (*msr_get_track_error)(int nTrackIndex);
typedef int (*msr_get_track_data_length)(int nTrackIndex);
typedef int (*msr_get_track_data)(int nTrackIndex, unsigned char* pTrackData, int nLength);

#ifdef __cplusplus
}
#endif

#endif /* MSR_INTERFACE_H_ */
