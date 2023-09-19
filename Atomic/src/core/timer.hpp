    #ifndef TIMER_HPP
    #define TIMER_HPP
	
     
    #include <chrono>
     
    template <typename T>
    class Timer
    {
    private:
    	std::chrono::time_point<std::chrono::high_resolution_clock> tStart;
    public:
     
    	Timer()
    	{
    		tStart = std::chrono::high_resolution_clock::now();
    	}
     
    	float diff()
    	{
    		return std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now() - tStart).count();
    	}
     
    	void reset()
    	{
    		tStart = std::chrono::high_resolution_clock::now();
    	}

		// High Resolution clock for time stamps and to delay a process in MilliSeconds
		uintptr_t GetMs() {
			using namespace std::chrono;
			milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			return static_cast<uintptr_t>(ms.count());
		}
    };
     
    #endif
