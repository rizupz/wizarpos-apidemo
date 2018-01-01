/*
 * contactless_card_jni_interface.h
 *
 *  Created on: 2012-7-24
 *      Author: yaomaobiao
 */

#ifndef CONTACTLESS_CARD_JNI_INTERFACE_H_
#define CONTACTLESS_CARD_JNI_INTERFACE_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

jint JNICALL native_contactless_card_open(JNIEnv*, jclass);
jint JNICALL native_contactless_card_close(JNIEnv*, jclass);
jint JNICALL native_contactless_card_poll_event(JNIEnv*, jclass, jint, jobject);
jint JNICALL native_contactless_card_search_target_begin(JNIEnv *, jclass, jint, jint, jint);
jint JNICALL native_contactless_card_search_target_end(JNIEnv *, jclass);
jint JNICALL native_contactless_card_attach_target(JNIEnv *, jclass, jbyteArray);
jint JNICALL native_contactless_card_detach_target(JNIEnv *, jclass);
jint JNICALL native_contactless_card_transmit(JNIEnv *, jclass, jbyteArray, jint, jbyteArray);
jint JNICALL native_contactless_card_send_control_command(JNIEnv *, jclass, jint, jbyteArray, jint);
jint JNICALL native_contactless_card_mc_verify_pin(JNIEnv *, jclass  , jint , jint, jbyteArray, jint);
jint JNICALL native_contactless_card_mc_read(JNIEnv *, jclass , jint , jint, jbyteArray, jint);
jint JNICALL native_contactless_card_mc_write(JNIEnv *, jclass  , jint , jint, jbyteArray, jint);
jint JNICALL native_contactless_card_query_info(JNIEnv *, jclass, jintArray, jintArray);
jint JNICALL native_contactless_card_mc_read_value(JNIEnv *, jclass, jint , jint, jbyteArray, jint, jbyteArray);
jint JNICALL native_contactless_card_mc_write_value(JNIEnv *, jclass, jint , jint, jint, jint, jbyte);
jint JNICALL native_contactless_card_mc_increment(JNIEnv *, jclass, jint , jint, jint);
jint JNICALL native_contactless_card_mc_decrement(JNIEnv *, jclass, jint , jint, jint);
jint JNICALL native_contactless_card_mc_transfer(JNIEnv *, jclass, jint , jint);
jint JNICALL native_contactless_card_mc_restore(JNIEnv *, jclass, jint , jint);

#ifdef __cplusplus
}
#endif


#endif /* CONTACTLESS_CARD_JNI_INTERFACE_H_ */
