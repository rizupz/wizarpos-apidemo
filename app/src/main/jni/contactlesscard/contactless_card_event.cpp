/*
 * contactless_card_event.cpp
 *
 *  Created on: 2012-7-24
 *      Author: yaomaobiao
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "hal_sys_log.h"

#include "contactless_card_event.h"

CContactlessCardEvent::CContactlessCardEvent()
{
	m_nEventID = -1;
	memset(m_strEventData, 0, MAX_CONTACTLESS_CARD_EVENT_DATA_LENGTH);
	m_nEventDataLength = 0;
}

CContactlessCardEvent::CContactlessCardEvent(const CContactlessCardEvent& event)
{
	m_nEventID = event.m_nEventID;
	memcpy(m_strEventData, event.m_strEventData, MAX_CONTACTLESS_CARD_EVENT_DATA_LENGTH);
	m_nEventDataLength = event.m_nEventDataLength;
}

CContactlessCardEvent::CContactlessCardEvent(int nEventID, unsigned char* pEventData, int nEventDataLength)
{
	m_nEventID = nEventID;
	m_nEventDataLength = nEventDataLength <= MAX_CONTACTLESS_CARD_EVENT_DATA_LENGTH ? nEventDataLength : MAX_CONTACTLESS_CARD_EVENT_DATA_LENGTH;
	memcpy(m_strEventData, pEventData, m_nEventDataLength);
}

CContactlessCardEvent::~CContactlessCardEvent()
{

}

void CContactlessCardEvent::explore()
{
	hal_sys_info("event id = %d\n", m_nEventID);
	hal_sys_info("event data length = %d\n", m_nEventDataLength);
	if(m_nEventDataLength > 0)
		hal_sys_dump("event data", m_strEventData, m_nEventDataLength);
	return;
}




