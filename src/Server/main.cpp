#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "counter.h"
#include "pool.h"
#include "Scheduler.h"
#include "Updater.h"

Core::Queue mapsQueue;

class MapUpdater : public Core::Updater
{
public:
	MapUpdater() : 
		Updater(&mapsQueue, 100)
	{}

	virtual int update() override
	{
		printf(".");
		fflush(stdout);

		Updater::update();
		//std::cout << "ID: " << std::this_thread::get_id() << std::endl;
	}
};

class SectorUpdater : public Core::Updater
{
public:
	SectorUpdater() : 
		Updater(&mapsQueue, 100)
	{}

	virtual int update() override
	{
		printf("/");
		fflush(stdout);

		Updater::update();
		//std::cout << "ID: " << std::this_thread::get_id() << std::endl;
	}
};

using time_base = std::chrono::microseconds;
using Scheduler = Core::Scheduler<time_base>;

Pool::ThreadPool* threadPool = nullptr;
Scheduler* scheduler = nullptr;

Core::Updater* mapUpdater_1 = nullptr;
Core::Updater* mapUpdater_2 = nullptr;

int main() {
	//Counter c;

	threadPool = Pool::ThreadPool::create(4);
	scheduler = Scheduler::create(500);

	mapUpdater_1 = new SectorUpdater();
	mapUpdater_2 = new MapUpdater();

	scheduler->every(100, mapUpdater_1);
	scheduler->every(50, mapUpdater_2);

	threadPool->permanent(Scheduler::update_handler, scheduler);

	getchar();

	printf("[END] Stopping all threads\n");

	threadPool->stop();
	threadPool->join();

	printf("[END] Cleaning memory\n");

	delete mapUpdater_1;
	delete mapUpdater_2;
	delete scheduler;
	delete threadPool;

	return 0;
}
