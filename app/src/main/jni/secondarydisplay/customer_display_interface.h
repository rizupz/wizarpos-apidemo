#ifndef JNI_CUSTOMER_DISPLAY__INTERFACE_H_
#define JNI_CUSTOMER_DISPLAY__INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*
* open device
* return value : NULL : faled in opening device.
* 				  else : handle of device
*/
typedef void* (*cusdisplay_open_ex)(int* pError);

typedef void* (*cusdisplay_open)();

/*
 * set display coordinate and width
 * @param[in] : int nHandle : handle of this device
 * @param[in] int nXcoordinate	: x coordinate.
 * @param[in] int nYcoordinate	: y coordinate.
 * @param[in] int nWidth		: 0:	full screen.
 *								  else: width(MAX: 480).
 * return value : 0 : success
 * 				  < 0 : error code
 */
//typedef int (*cusdisplay_set_coordinate)(int nHandle,unsigned int nXcoordinate,unsigned int nYcoordinate,unsigned int nWidth);

/*
 * write picture point data (one point 3 bytes RGB)
 * @param[in] : int nHandle		: handle of this device.
 * @param[in] unsigned char* pData		: all point data.
 * @param[in] unsigned int nDataLength	: point data length.
 * return value : 0 : success
 * 				  < 0 : error code
 */
//typedef int (*cusdisplay_write_picture_data)(int nHandle,unsigned char* pData, unsigned int nDataLength);

/*
 * write picture point data (one point 4 bytes ARGB8888)
 * @param[in] : int nHandle		: handle of this device.
 * @param[in] unsigned int nXcoordinate	: x coordinate.
 * @param[in] unsigned int nYcoordinate	: y coordinate.
 * @param[in] unsigned int nWidth		: 0:	full screen.
 *								  		  else: width.
 * @param[in] unsigned int nHeight      : 0:	full screen.
 *								  		  else: height.
 * @param[in] unsigned char* pData		: all point data.
 * @param[in] unsigned int nDataLength	: point data length.
 * return value : 0 : success
 * 				  < 0 : error code
 */
typedef int (*cusdisplay_write_picture_data)(int nHandle, unsigned int nXcoordinate, unsigned int nYcoordinate,unsigned int nWidth, unsigned int nHeight ,unsigned char* pData, unsigned int nDataLength);

//typedef int (*set_background)(int nHandle,int nColor);

//typedef int (*buzzer_beep)(int nHandle);

//typedef int (*led_power)(int nHandle, int nValue);

//typedef int (*display_default_screen)(int nHandle);
/*
 * open or close led,buzzer beep and so on.
 * @param[in] : int nHandle		: handle of this device.
 * @param[in] : int nCmd		: contral command. 0x04:clean screen.
 *												   0x05:default screen.
 *												   0x06:led.
 *												   0x07:buzzer.
 * @param[in] int nValue		: clean  	RED :0x001F   BLACK:0X0000   YELLOW:0X07FF    BLUE:0XF800    GRAY0:0XCE9A
 *								  default   0
 *								  led 		1:open led.
 *								  	  		0:close led.
 *								  buzzer  	0
 * return value : 0 : success
 * 				  < 0 : error code
 */
typedef int (*cusdisplay_ctrl_devs)(int nHandle, int nCmd, int nValue);

/*
 * close the device
 * @param[in] : int nHandle : handle of this device
 * return value : 0 : success
 * 				  < 0 : error code
 */
typedef int (*cusdisplay_close)(int nHandle);

#ifdef __cplusplus
}
#endif

#endif
