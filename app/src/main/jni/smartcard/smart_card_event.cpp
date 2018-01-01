/*
 * smart_card_event.cpp
 *
 *  Created on: 2012-8-2
 *      Author: yaomaobiao
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "hal_sys_log.h"
#include "smart_card_event.h"

CSmartCardEvent::CSmartCardEvent()
{
	m_nEventID = -1;
	m_nSlotIndex = -1;
}


CSmartCardEvent::CSmartCardEvent(int nEventID, int nSlotIndex)
{
	m_nEventID = nEventID;
	m_nSlotIndex = nSlotIndex;
}

CSmartCardEvent::CSmartCardEvent(const CSmartCardEvent& event)
{
	m_nEventID = event.m_nEventID;
	m_nSlotIndex = event.m_nSlotIndex;
}
void CSmartCardEvent::explore()
{
	hal_sys_info("event id = %d\n", m_nEventID);
	hal_sys_info("slot index = %d\n", m_nSlotIndex);
}


