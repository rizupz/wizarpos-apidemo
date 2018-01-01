/*
 * printer_interface.h
 *
 *  Created on: 2012-7-19
 *      Author: yaomaobiao
 */

#ifndef PRINTER_INTERFACE_H_
#define PRINTER_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * open the device
 * return value : 0 : success
 * 				  < 0 : error code
 */
typedef int (*printer_open)();
/*
 * prepare to print
 * return value : 0 : success
 * 				  < 0 : error code
 */
typedef int (*printer_begin)();
/*
 * end to print
 * return value : 0 : success
 * 				  < 0 : error code
 */
typedef int (*printer_end)();
/*
 * write the data to the device
 * @param[in] : unsigned char* pData, data or control command
 * @param[in] : int nDataLength : length of data or control command
 * return value : 0 : success
 * 				  < 0 : error code
 */
typedef int (*printer_write)(unsigned char* pData, int nDataLength);

/*
 * query the status of printer
 * return value : < 0 : error code
 *                = 0 : no paper
 *                = 1 : has paper
 *                other value : RFU
 */
typedef int (*printer_query_status)();
/*
 * close the device
 * return value : 0 : success
 * 				  < 0 : error code
 */

typedef int (*printer_close)();

typedef int (*printer_query_voltage)(int *capacity, int *voltage);

typedef int (*printer_sync)();

#ifdef __cplusplus
}
#endif


#endif /* PRINTER_INTERFACE_H_ */
