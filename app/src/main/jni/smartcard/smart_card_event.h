/*
 * smart_card_event.h
 *
 *  Created on: 2012-8-2
 *      Author: yaomaobiao
 */

#ifndef SMART_CARD_EVENT_H_
#define SMART_CARD_EVENT_H_

class CSmartCardEvent
{
public:
	CSmartCardEvent();
	CSmartCardEvent(int nEventID, int nSlotIndex);
	CSmartCardEvent(const CSmartCardEvent& event);
public:
	void explore();
public:
	int m_nEventID;
	int m_nSlotIndex;
};

#endif /* SMART_CARD_EVENT_H_ */
