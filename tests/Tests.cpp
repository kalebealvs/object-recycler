#include <gmock/gmock.h>

#include "Recycler.h"

using namespace ::testing;

class Example {
public:
    Example () {++id;};
    static int id;
};

int Example::id = 0;

TEST(RecyclerTests, HaveOnlyOneInstancePerRecycler) {
    auto recycler = Recycler<Example>::getInstance();
    auto recycler2 = Recycler<Example>::getInstance();
    ASSERT_THAT(recycler, Eq(recycler2));
}