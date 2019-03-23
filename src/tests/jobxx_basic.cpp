#include "catch2/catch.hpp"
#include "jobxx/queue.h"
#include "jobxx/park.h"

#include <thread>
#include <future>
#include <atomic>

// This is a simple thread pool setup for testing jobxx
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
				std::future_status result = future->wait_for(std::chrono::seconds(5));

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

SCENARIO("Tasks are executed", "[jobxx]")
{
	TestThreadPool threadPool;

	GIVEN("Some number of tasks with no dependencies")
	{
		unsigned taskCount = GENERATE(0, 1, 5, 10, 20, 30, 100, 200);

		std::mutex taskCompleteMutex;
		std::condition_variable allTasksCompleted;
		std::atomic<int> executedCount = 0;
		auto task = [&]()
		{
			// sleep for short time
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			++executedCount;
			{
				std::lock_guard<std::mutex> guard(taskCompleteMutex);
				allTasksCompleted.notify_one();
			}
		};

		WHEN("I execute the task(s)")
		{
			unsigned success = 0;
			for (unsigned i = 0; i < taskCount; ++i)
			{
				if (threadPool.Queue().spawn_task(task) == jobxx::spawn_result::success)
				{
					++success;
				}
			}

			THEN("They should have spawned correctly")
			{
				REQUIRE(success == taskCount);
			}

			// wait until tasks complete
			allTasksCompleted.wait(std::unique_lock<std::mutex>(taskCompleteMutex), [&] { return executedCount == taskCount; });

			THEN("They should have all executed only once")
			{
				REQUIRE(executedCount == taskCount);
			}
		}
	}
}

SCENARIO("Jobs are executed and completed", "[jobxx]")
{
	TestThreadPool threadPool;

	GIVEN("A job with some number of tasks")
	{
		unsigned taskCount = GENERATE(0, 1, 5, 10, 20, 30, 100, 200);

		std::atomic<int> executedCount = 0;
		auto jobInit = [&](jobxx::context& context)
		{
			for (unsigned i = 0; i < taskCount; ++i)
			{
				context.spawn_task([&]()
				{
					// sleep for a short time
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					++executedCount;
				});
			}
		};

		WHEN("I execute the job")
		{
			jobxx::job job = threadPool.Queue().create_job(jobInit);

			THEN("The job should not complete immediately")
			{
				if (taskCount > 0)
					REQUIRE_FALSE(job.complete());
			}

			// wait until the job completes
			while (!job.complete()) {}

			THEN("The job should complete")
			{
				REQUIRE(job.complete());
			}

			THEN("The job should execute the correct number of times when it's completed")
			{
				REQUIRE(executedCount == taskCount);
			}
		}
	}
}
