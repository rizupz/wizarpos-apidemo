/*
 * printer_jni_interface.h
 *
 *  Created on: 2012-8-3
 *      Author: yaomaobiao
 */

#ifndef PRINTER_JNI_INTERFACE_H_
#define PRINTER_JNI_INTERFACE_H_

const char* printer_get_class_name();
//	+add by pengli
const char* get_class_name_internal();
//	-add by pengli
JNINativeMethod* printer_get_methods(int* pCount);

#endif /* PRINTER_JNI_INTERFACE_H_ */
