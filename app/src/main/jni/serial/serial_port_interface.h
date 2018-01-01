#ifndef SERIAL_PORT_INTERFACE_H_
#define SERIAL_PORT_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * open the serial port
 * @param[in] : char* pDeviceName : device name
 * return value : < 0 : error code
 * 				  >= 0 : file description
 */
typedef int (*esp_open)(char* pDeviceName);

/*
 * close the serial port
 * @param[in] : int fd : file description
 * return value : < 0 : error code
 * 				  >=0 : success
 */
typedef int (*esp_close)(int fd);

/*
 * read data from the serial port
 * @param[in] : int fd : file description
 * @param[in] : unsigned char* pDataBuffer : data buffer
 * @param[in] : int ExpectedDataLength : data length of reading
 * @param[in] : int nTimeout_MS : time out in milliseconds
 * 								  nTimeout_MS : 0 : read and return immediately
 * 								  				< 0 : read until we have received data
 * return value : < 0 : error code
 * 				  >= 0 : data length
 */
typedef int (*esp_read)(int fd, unsigned char* pDataBuffer, int nExpectedDataLength, int nTimeout_MS);

/*
 * write data to the serial port
 * @param[in] : int fd : file description
 * @param[in] : unsigned char* pDataBuffer : data buffer
 * @param[in] : int nDataLength : data length
 * return value : < 0 : error code
 * 				  >= 0 : written data length
 */
typedef int (*esp_write)(int fd, unsigned char* pDataBuffer, int nDataLength);

/*
 * set the bard rate
 * @param[in] : int fd : file description
 * return value : < 0 : error code
 * 				  >= 0 :
 */
typedef int (*esp_set_baudrate)(int fd, unsigned int nBaudrate);

/*
 * flush io buffer
 * @param[in] : int fd : file description
 * return value : < 0 : error code
 * 				  >= 0 :
 */
typedef int (*esp_flush_io)(int fd);


#ifdef __cplusplus
}
#endif

#endif /* SERIAL_PORT_INTERFACE_H_ */
