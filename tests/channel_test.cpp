#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "channel.hpp"

using namespace pubsub;

TEST(ChannelTest, PushAndPop) {
    Channel<int> ch;
    ch.push(1);
    auto val = ch.pop();
    ASSERT_TRUE(val);
    EXPECT_EQ(*val, 1);
}

TEST(ChannelTest, PopBlocksUntilPush) {
    Channel<std::string> ch;
    std::string result;
    std::thread t([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ch.push("hello");
    });

    auto val = ch.pop();
    ASSERT_TRUE(val);
    EXPECT_EQ(*val, "hello");
    t.join();
}

TEST(ChannelTest, CloseReturnsNullopt) {
    Channel<int> ch;
    ch.close();
    auto val = ch.pop();
    EXPECT_FALSE(val);
}

TEST(ChannelTest, PushAfterCloseIgnored) {
    Channel<int> ch;
    ch.close();
    ch.push(1);
    auto val = ch.pop();
    EXPECT_FALSE(val);
}