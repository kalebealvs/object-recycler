#include <gmock/gmock.h>
#include <memory>
#include <atomic>

#include "Recycler.h"

using namespace ::testing;

class Example {
public:
    Example () {++_id_count;};
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