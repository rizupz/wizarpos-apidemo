/*
 * pinpad_jni_interface.h
 *
 *  Created on: 2012-8-8
 *      Author: yaomaobiao
 */

#ifndef PINPAD_JNI_INTERFACE_H_
#define PINPAD_JNI_INTERFACE_H_

const char* pinpad_get_class_name();
//	+add by pengli
const char* get_class_name_internal();
//	-add by pengli
JNINativeMethod* pinpad_get_methods(int* pCount);

#endif /* PINPAD_JNI_INTERFACE_H_ */
