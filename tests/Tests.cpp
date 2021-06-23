#include <gmock/gmock.h>
#include <atomic>
#include <chrono>
#include <exception>
#include <memory>
#include <shared_mutex>
#include <thread>

#include "Recycler.h"

using namespace ::testing;

class Example {
public:
    Example () : id{++_id_count} {};
    const uint id;
    static std::atomic<uint> _id_count;
};

std::atomic<uint> Example::_id_count = 0;

struct RecyclerTests : public Test {
    std::shared_ptr<Recycler<Example>> recycler = Recycler<Example>::getInstance();
};

TEST_F(RecyclerTests, HaveOnlyOneInstancePerRecycler) {
    auto recycler2 = Recycler<Example>::getInstance();
    ASSERT_THAT(recycler, Eq(recycler2));
}

TEST_F(RecyclerTests, CreatesInstanceIfRecyclerEmpty) {
    const uint old_count{Example::_id_count};
    auto instance = recycler->recycle();
    const uint new_count{Example::_id_count};
    ASSERT_THAT(new_count, Gt(old_count));
}

TEST_F(RecyclerTests, RecyclesExistingInstance) {
    auto instance = recycler->recycle();
    auto* instance_address = instance.get();
    recycler->trash(std::move(instance));
    auto recycled = recycler->recycle();
    auto* recycled_address = recycled.get();
    ASSERT_THAT(recycled_address, Eq(instance_address));
}

TEST_F(RecyclerTests, ThreadSafeRecycling) {
    std::shared_mutex mutex;
    std::vector<std::thread> threads;
    try {
        {
            std::scoped_lock lock (mutex);
            const auto worker = [this, &mutex](const int id) {
                const int iterations{100000};
                const int less_iterations{iterations / 8};
                std::shared_lock concurrent_lock (mutex);
                if (id % 2 == 0) {
                    for (int i = 0; i < iterations; ++i)
                        recycler->trash(recycler->recycle());
                } else {
                    std::vector<std::unique_ptr<Example>> items;
                    items.reserve (less_iterations);
                    for (int base = 0; base < 4; ++base) {
                        for (int i = 0; i < less_iterations; ++i)
                            items.push_back(recycler->recycle());
                        
                        for (auto& item : items)
                            recycler->trash(std::move(item));

                        items.clear();
                    }
                }
            };
            
            const auto cpu_count = std::thread::hardware_concurrency();
            if (cpu_count == 0)
                FAIL();

            for (uint cpu = 0; cpu <  cpu_count; ++cpu)
                threads.emplace_back (worker, cpu);

            std::this_thread::sleep_for (std::chrono::milliseconds(400));
        }

        for (auto& thread : threads)
            thread.join();
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
        FAIL();
    }
}