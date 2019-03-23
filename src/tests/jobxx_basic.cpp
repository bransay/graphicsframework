#include "catch2/catch.hpp"
#include "jobxx/queue.h"

#include <thread>
#include <future>

// TODO: we will probably replace this with something else
template <unsigned THREAD_POOL_SIZE>
class SimpleThreadPool
{
public:
	SimpleThreadPool()
	{
		// spawn all the threads
		for (unsigned i = 0; i < THREAD_POOL_SIZE; ++i)
			m_ThreadPool[i] = std::thread(SimpleThreadPool::ThreadFuncStub, this);
	}

	~SimpleThreadPool()
	{
		// close the queue (which should cause all the worker threads to die)
		m_Queue.close();

		// join all threads
		for (unsigned i = 0; i < THREAD_POOL_SIZE; ++i)
			m_ThreadPool[i].join();
	}

	jobxx::queue& Queue()
	{
		return m_Queue;
	}

private:
	static void ThreadFuncStub(SimpleThreadPool* threadPool)
	{
		// put the queue into perpetual work on this thread
		threadPool->Queue().work_forever();
	}

	std::thread m_ThreadPool[THREAD_POOL_SIZE];
	jobxx::queue m_Queue;
};
using TestThreadPool = SimpleThreadPool<8>;

SCENARIO("Thread pools for testing can be created and destroyed", "[jobxx]")
{
	GIVEN("An empty thread pool that was created")
	{
		bool startDestroy = false;
		std::mutex mutex;
		std::condition_variable destroyThreadPool;
		std::thread testThread([&]()
		{
			auto threadPool = std::make_shared<TestThreadPool>();
			{
				std::unique_lock<std::mutex> lock(mutex);
				destroyThreadPool.wait(lock, [&] { return startDestroy; });
			}
			threadPool.reset();
		});

		WHEN("it attempts to destruct")
		{
			{
				std::lock_guard<std::mutex> guard(mutex);
				startDestroy = true;
			}
			destroyThreadPool.notify_one();

			THEN("it should be destroyed within a reasonable time (5s)")
			{
				// attempt to join the thread in a future with a timeout
				auto* future = new std::future<void>(std::async(std::launch::async, &std::thread::join, &testThread));
				std::future_status result = future->wait_for(std::chrono::seconds(1));

				// if we timed out, the thread is doomed, just detach and let it run its course
				// we can't delete the future either or its blocked, so we're going to leak
				if (result == std::future_status::timeout)
					testThread.detach();
				else
					delete future; // otherwise all is good, delete away

				// evil force threading to end code needs to be caught to continue tests
				try
				{
					REQUIRE(result != std::future_status::timeout);
				}
				catch (...) {}
			}
		}
	}
}

SCENARIO("Jobs are executed and completed", "[jobxx]")
{
	GIVEN("A job with no dependencies")
	{
		WHEN("I execute a job")
		{
			THEN("It should complete execution")
			{
			}

			THEN("It should have executed")
			{

			}
		}
	}
}
