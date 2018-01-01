#ifndef CUSTOMER_DISPLAY_JNI_INTERFACE_H_
#define CUSTOMER_DISPLAY_JNI_INTERFACE_H_

const char* customer_display_get_class_name_allinpay();
//	+add by pengli
const char* get_class_name_internal();
//	-add by pengli
JNINativeMethod* customer_display_get_methods(int* pCount);

#endif 
