    #ifndef TIMER_H
    #define TIMER_H
     
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
    };
     
    #endif
