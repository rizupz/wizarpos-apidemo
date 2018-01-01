#ifndef MONEY_SERVICE_INTERFACE_H_
#define MONEY_SERVICE_INTERFACE_H_

const char* money_box_get_class_name();
//	+add by pengli
const char* get_class_name_internal();
//	=add by pengli
JNINativeMethod* money_box_get_methods(int* pCount);

#endif /* MONEY_SERVICE_INTERFACE_H_ */
