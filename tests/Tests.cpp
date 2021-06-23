#include <gmock/gmock.h>
#include <memory>
#include <atomic>

#include "Recycler.h"

using namespace ::testing;

class Example {
public:
    Example () : id{++_id_count} {};
    const int id;
    static std::atomic<int> _id_count;
};

std::atomic<int> Example::_id_count = 0;

struct RecyclerTests : public Test {
    std::shared_ptr<Recycler<Example>> recycler = Recycler<Example>::getInstance();
};

TEST_F(RecyclerTests, HaveOnlyOneInstancePerRecycler) {
    auto recycler2 = Recycler<Example>::getInstance();
    ASSERT_THAT(recycler, Eq(recycler2));
}

TEST_F(RecyclerTests, CreatesInstanceIfRecyclerEmpty) {
    const int old_count{Example::_id_count};
    auto instance = recycler->recycle();
    const int new_count{Example::_id_count};
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