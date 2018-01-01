#ifndef IDENTITY_CARD_JNI_INTERFACE_H_
#define IDENTITY_CARD_JNI_INTERFACE_H_

const char* identity_card_get_class_name();
//	+add by pengli
const char* get_class_name_internal();
//	-add by pengli
JNINativeMethod* identity_card_get_methods(int* pCount);
//	+add by pengli
JNINativeMethod* get_methods_internal(int* pCount);
//	-add by pengli

#endif /* IDENTITY_CARD_JNI_INTERFACE_H_ */
