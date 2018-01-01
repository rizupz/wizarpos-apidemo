#ifndef SERIAL_PORT_JNI_INTERFACE_H_
#define SERIAL_PORT_JNI_INTERFACE_H_

const char* serial_port_get_class_name();
//	+add by pengli
const char* get_class_name_internal();
//	-add by pengli
JNINativeMethod* serial_port_get_methods(int* pCount);

#endif /* SERIAL_PORT_JNI_INTERFACE_H_ */
