/*
 * joining_thread.h
 *
 *  Created on: May 4, 2014
 *      Author: lukas
 */

#ifndef JOINING_THREAD_H
#define JOINING_THREAD_H
#include <thread>
class joining_thread{


public:

	template <typename Function, typename... Args>
	    explicit joining_thread(Function&& f, Args&&... args)
	        : mThread(std::forward<Function>(f), std::forward<Args>(args)...)
	    {}

	~joining_thread(){
		mThread.join();
	}

	//copy semantik
	joining_thread(joining_thread& other ){}
	joining_thread& operator=(const joining_thread& other ){return *this;}

	//move semantik
	joining_thread(joining_thread&& other ):mThread(std::move(other.mThread)){}
	joining_thread& operator=(joining_thread&& other){
		mThread=std::move(other.mThread);
		return *this;
	}

private:
	std::thread mThread;
};



#endif /* JOINING_THREAD_H_ */
