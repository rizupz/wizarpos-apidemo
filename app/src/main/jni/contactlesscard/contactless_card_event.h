/*
 * contactless_card_event.h
 *
 *  Created on: 2012-7-24
 *      Author: yaomaobiao
 */

#ifndef CONTACTLESS_CARD_EVENT_H_
#define CONTACTLESS_CARD_EVENT_H_

#define MAX_CONTACTLESS_CARD_EVENT_DATA_LENGTH		64

class CContactlessCardEvent
{
public:
	CContactlessCardEvent();
	CContactlessCardEvent(const CContactlessCardEvent& event);
	CContactlessCardEvent(int nEventID, unsigned char* pEventData, int nEventDataLength);
	~CContactlessCardEvent();
public:
	void explore();
public:
	int m_nEventID;
	unsigned char m_strEventData[MAX_CONTACTLESS_CARD_EVENT_DATA_LENGTH];
	int m_nEventDataLength;

};


#endif /* CONTACTLESS_CARD_EVENT_H_ */
