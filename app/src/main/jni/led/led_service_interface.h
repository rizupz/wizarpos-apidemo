#ifndef LED_SERVICE_INTERFACE_H
#define LED_SERVICE_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * open the led device
 * @return value : < 0 : error code
 * 				   >= 0 : success;	
 */
typedef int (*LED_OPEN)(void);
/*
 * close the led device
 * @return value : < 0 : error code
 * 				   >= 0 : success;
 */
typedef int (*LED_CLOSE)(void);

/*
 * turn on the led
 * @param[in] : unsigned int nLedIndex : index of led, >= 0 && < MAX_LED_COUNT
 * @return value : < 0 : error code;
 *                 >= 0 : success
 */
typedef int (*LED_ON)(unsigned int nLedIndex);
/*
 * turn off the led
 * @param[in] : unsigned int nLedIndex : index of led, >= 0 && < MAX_LED_COUNT
 * @return value : < 0 : error code;
 *                 >= 0 : success
 */

typedef int (*LED_OFF)(unsigned int nLendIndex);
/*
 * get the status of led
 * @param[in] : unsigned int nLedIndex : index of led, >= 0 && < MAX_LED_COUNT
 * @return value : == 0 : turn off
 *                 > 0 : turn on
 *                 < 0 : error code
 */
typedef int (*LED_GET_STATUS)(unsigned int nLedIndex);

#ifdef __cplusplus
}
#endif


#endif
