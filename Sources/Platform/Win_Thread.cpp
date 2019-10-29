#include "stdafx.h"
#pragma hdrstop

//
#ifdef OS_LINUX

void	T_CriticalSection::Create() {
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
	pthread_mutex_init(&Section, &attr);

	pthread_mutexattr_destroy(&attr);
}

void	T_CriticalSection::Destroy() {
	pthread_mutex_destroy(&Section);
}

bool	T_CriticalSection::Try() {
	if (pthread_mutex_trylock(&Section) != 0) {
		return false;
	}
	return true;
}

void	T_CriticalSection::Lock() {
	if (pthread_mutex_trylock(&Section) != 0) {
		pthread_mutex_lock(&Section);
	}
}

void	T_CriticalSection::Unlock() {
	pthread_mutex_unlock(&Section);
}

#endif

#ifdef OS_WINDOW

void	T_CriticalSection::Create() {
	InitializeCriticalSection(&Section);
}

void	T_CriticalSection::Destroy() {
	DeleteCriticalSection(&Section);
}

bool	T_CriticalSection::Try() {
	if (TryEnterCriticalSection(&Section) == 0) return false;
	return true;
}

void	T_CriticalSection::Lock() {
	if (TryEnterCriticalSection(&Section) == 0) {
		EnterCriticalSection(&Section);
	}
}

void	T_CriticalSection::Unlock() {
	LeaveCriticalSection(&Section);
}

#endif
//
#ifdef OS_LINUX

void	T_Mutex::Create(bool _manualReset) {
	manualReset = _manualReset;
	signaled = false;
	waiting = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
}

void	T_Mutex::Destroy() {
	signaled = false;
	waiting = 0;
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

void	T_Mutex::Raise() {
	pthread_mutex_lock(&mutex);

	if (manualReset) {
		signaled = true;
		pthread_cond_broadcast(&cond);
	}
	else {
		if (waiting > 0)	pthread_cond_signal(&cond);
		else				signaled = true;
	}

	pthread_mutex_unlock(&mutex);
}

void	T_Mutex::Clear() {
	pthread_mutex_lock(&mutex);
	signaled = false;
	pthread_mutex_unlock(&mutex);
}

bool	T_Mutex::Wait(u32 timeout) {
	int status;
	pthread_mutex_lock(&mutex);

	if (signaled) {
		if (!manualReset) signaled = false;
		status = 0;
	}
	else {
		++waiting;
		if (timeout == WAIT_INFINITE) {
			status = pthread_cond_wait(&cond, &mutex);
		}
		else {
			timespec ts;

			clock_gettime(CLOCK_REALTIME, &ts);

			ts.tv_nsec += (timeout % 1000) * 1000000;
			ts.tv_sec += timeout / 1000;
			if (ts.tv_nsec >= 1000000000) {
				ts.tv_nsec -= 1000000000;
				ts.tv_sec += 1;
			}

			status = pthread_cond_timedwait(&cond, &mutex, &ts);
		}
		--waiting;
	}

	pthread_mutex_unlock(&mutex);

	assert(status == 0 || (timeout != WAIT_INFINITE && status == ETIMEDOUT));

	return (status == 0);
}

bool	T_Mutex::WaitInfinite() {
	return Wait(WAIT_INFINITE);
}

#endif


#ifdef OS_WINDOW

void	T_Mutex::Create(bool _manualReset) {
	Handle = CreateEvent(NULL, _manualReset, FALSE, NULL);
}

void	T_Mutex::Destroy() {
	CloseHandle(Handle);
}

void	T_Mutex::Raise() {
	SetEvent(Handle);
}

void	T_Mutex::Clear() {
	ResetEvent(Handle);
}

bool	T_Mutex::Wait(i32 timeout) {
	DWORD result = WaitForSingleObject(Handle, timeout == -1 ? INFINITE : timeout);
	return (result == WAIT_OBJECT_0);
}

bool	T_Mutex::WaitInfinite() {
	DWORD result = WaitForSingleObject(Handle, INFINITE);
	return (result == WAIT_OBJECT_0);
}

#endif
//

#ifdef OS_LINUX

void				T_LockedInt::Init() {
	value = 0;
}

int					T_LockedInt::Increment() {
	return __sync_add_and_fetch(&value, 1);
}

int					T_LockedInt::Decrement() {
	return __sync_sub_and_fetch(&value, 1);
}

int					T_LockedInt::Add(int v) {
	return __sync_add_and_fetch(&value, v);
}

int					T_LockedInt::Sub(int v) {
	return __sync_sub_and_fetch(&value, v);
}

int					T_LockedInt::Get() {
	return value;
}

void				T_LockedInt::Set(int v) {
	value = v;
}

T_LockedInt			T_LockedInt::operator=(const int  v) {
	value = v;
}

const int			T_LockedInt::operator=(T_LockedInt  v) {
	return value;
}

////return InterlockedExchange( & value, exchange );
//return __sync_val_compare_and_swap(&value, value, exchange);
////return InterlockedCompareExchange( & value, exchange, comparand );
//return __sync_val_compare_and_swap(&value, comparand, exchange);

#endif

#ifdef OS_WINDOW

void				T_LockedInt::Init() {
	value = 0;
}

int					T_LockedInt::Increment() {
	return InterlockedIncrementAcquire(&value);
}

int					T_LockedInt::Decrement() {
	return InterlockedDecrementRelease(&value);
}

int					T_LockedInt::Add(int v) {
	return InterlockedExchangeAdd(&value, (LONG)v) + (LONG)v;
}

int					T_LockedInt::Sub(int v) {
	return InterlockedExchangeAdd(&value, -(LONG)v) - (LONG)v;
}

int					T_LockedInt::Get() {
	return value;
}

void				T_LockedInt::Set(int v) {
	value = (LONG)v;
}

void			T_LockedInt::operator=(const int  v) {
	value = (LONG)v;
}

const int			T_LockedInt::operator=(T_LockedInt  v) {
	return value;
}

#endif
//
#ifdef OS_LINUX

H_THREAD			Threads_Create(threadfunction function, void* parms, PConstStr Name){
	H_THREAD res;

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) != 0)						ERROR("pthread_attr_setdetachstate %s failed\n", Name);

	if (pthread_create((pthread_t*)&res, &attr,function, parms) != 0)		ERROR("pthread_create %s failed\n", Name);
	
	pthread_attr_destroy(&attr);
	
	return res;
}

void				Threads_Destroy(H_THREAD info){
	if (pthread_cancel((pthread_t)info) != 0)		ERROR("pthread_cancel failed\n");	// ƒосрочное завершение потока
	if (pthread_join((pthread_t)info, NULL) != 0)	ERROR("pthread_join failed\n");		// ожидает завершени¤ потока
}

void				Threads_Terminate(H_THREAD h) {
	if (pthread_cancel((pthread_t)h) != 0)		ERROR("pthread_cancel  failed\n");	// ƒосрочное завершение потока
}


void				Thread_Yield() {
	pthread_yield();
}

void				Thread_Sleep(u32 ms) {
	usleep(ms * 1000);
}

H_THREAD			Thread_GetCurrent() {
	return pthread_self();
}

void				Thread_Exit() {
	pthread_exit(0);
}

void				Thread_TextExit() {
	pthread_testcancel();
}

#endif



#ifdef OS_WINDOW

H_THREAD			Threads_Create(threadfunction function, void* parms, PConstStr Name) {
	
	DWORD threadId;
	H_THREAD res = CreateThread(NULL, 256 * 1024, (LPTHREAD_START_ROUTINE)function, parms, STACK_SIZE_PARAM_IS_A_RESERVATION, &threadId);

	return res;
}

void				Threads_Destroy(H_THREAD info) {
	if (info == 0) return;
	WaitForSingleObject(info, INFINITE);
	CloseHandle(info);
}

void				Threads_Terminate(H_THREAD h) {
#pragma warning(suppress: 6258)
	TerminateThread(h, 0);
	CloseHandle(h);
}


void				Thread_Yield() {
	SwitchToThread();
}

void				Thread_Sleep(u32 ms) {
	Sleep(ms);
}

H_THREAD			Thread_GetCurrent() {
	return GetCurrentThread();
}

void				Thread_Exit() {
	ExitThread(0);
}

void				Thread_TextExit() {
	
}

#endif