/*
 * event_manager.h
 *
 *  Created on: 2012-7-23
 *      Author: yaomaobiao
 */

#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include <errno.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>

#include <deque>

#include "hal_sys_log.h"

using namespace std;


template<typename T>
class CEventQueue
{
public:
	CEventQueue()
	{
		sem_init(&m_sem, 0, 0);
		pthread_mutex_init(&(m_accessMutex), NULL);
	}
	~CEventQueue()
	{
		sem_destroy(&m_sem);
		pthread_mutex_destroy(&(m_accessMutex));
	}
public:
	int push_back(const T& item)
	{
		int nValue = 0;
		pthread_mutex_lock(&m_accessMutex);
		m_data.push_back(item);
		hal_sys_info("current deque size = %d\n", m_data.size());
		pthread_mutex_unlock(&m_accessMutex);
		sem_post(&m_sem);
		return 0;
	}

	int clear()
		{
			pthread_mutex_lock(&m_accessMutex);
			m_data.clear();
			hal_sys_info("current deque size = %d\n", m_data.size());
			pthread_mutex_unlock(&m_accessMutex);
			return 0;
		}

	int pop_front(T& item, int nTimeout_MS = -1)
	{
		int nValue = 0;
		int nResult = -1;
		nResult = sem_wait_i(&m_sem, nTimeout_MS);
		if(nResult < 0)
			return nResult;

		pthread_mutex_lock(&m_accessMutex);
		if(m_data.size() == 0)
		{
			pthread_mutex_unlock(&m_accessMutex);
			return -1;
		}
		item = m_data.front();
		m_data.pop_front();
		pthread_mutex_unlock(&m_accessMutex);
		return 0;
	}
private:
	int sem_wait_i(sem_t* pSem, int nTimeout_MS)
	{
		int nResult = -1;
		struct timespec ts;
		if(nTimeout_MS >= 0)
		{
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec += nTimeout_MS / 1000;;
			ts.tv_nsec += (nTimeout_MS % 1000) * 1000;
		}
		while(1)
		{
			nResult = nTimeout_MS >= 0 ? sem_timedwait(&m_sem, &ts)
					:sem_wait(&m_sem);
			if(nResult == -1 && errno == EINTR)
				continue;
			else
				break;
		}
		if(nResult == -1)
		{
			/*
			if(errno == ETIMEDOUT)
				hal_sys_info("sem_timedwait() time out\n");
			else
				hal_sys_info(strerror(errno));
			*/
			if(errno != ETIMEDOUT)
			{
				hal_sys_info(strerror(errno));
			}
			return -errno;
		}
		return 0;
	}
private:
	deque<T> m_data;
	sem_t m_sem;
	pthread_mutex_t m_accessMutex;

};

#endif /* EVENT_QUEUE_H_ */
