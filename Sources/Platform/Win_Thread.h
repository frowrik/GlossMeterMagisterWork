


class T_CriticalSection {
public:
	void				Create();
	void				Destroy();

	bool				Try();							// false если занято
	void				Lock();
	void				Unlock();
private:
#ifdef OS_WINDOW
	CRITICAL_SECTION	Section;
#endif
#ifdef OS_LINUX
	pthread_mutex_t		Section;
#endif
};

#define	WAIT_INFINITE		0xffffffff

class T_Mutex {
public:
	void				Create(bool _manualReset = false);  // manualReset ручной управление отключено/ не сигнальное
	void				Destroy();

	void				Raise();							// ”станавливает указанное событие объекта в сигнальное состо¤ние.
	void				Clear();							// ”станавливает указанное событие объекта в несигнальное состо¤ние.
	bool				Wait(i32 timeout);					//  ќжидает, пока указанный объект будет находитьс¤ в сигнальном состо¤нии или пока врем¤ ожидани¤ не истекло. //false-кончилось врем¤//true-пришол сигнал
	bool				WaitInfinite();
private:
#ifdef OS_WINDOW
	HANDLE				Handle;
#endif
#ifdef OS_LINUX
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	bool				manualReset;
	bool				signaled;
	i32					waiting = 0;
#endif
};


class T_LockedInt {
public:
	void				Init();
	int					Increment();
	int					Decrement();
	int					Add(int v);
	int					Sub(int v);
	int					Get();
	void				Set(int v);
	void			operator=(const int  v);
	const int			operator=(T_LockedInt  v);
private:
#ifdef OS_WINDOW
	volatile LONG		value; // nead testing
#endif
#ifdef OS_LINUX
	volatile int		value;
#endif
};

//T_LockedPtr ptr
////return InterlockedExchangePointer( & ptr, exchange );
//return __sync_val_compare_and_swap(&ptr, ptr, exchange);
////return InterlockedCompareExchangePointer( & ptr, exchange, comparand );
//return __sync_val_compare_and_swap(&ptr, comparand, exchange);

typedef void* (*threadfunction)(void *);

#ifdef OS_WINDOW
typedef		HANDLE				H_THREAD;
#endif

#ifdef OS_LINUX
typedef		pthread_t			H_THREAD;
#endif


H_THREAD			Threads_Create(threadfunction function, void* parms, PConstStr Name="");
void				Threads_Destroy(H_THREAD info);
void				Threads_Terminate(H_THREAD h); // сразу рубим

void				Thread_Exit();
void				Thread_Yield();
void				Thread_Sleep(u32 ms);
H_THREAD			Thread_GetCurrent();
void				Thread_TextExit(); // для unix  что можно выйти

