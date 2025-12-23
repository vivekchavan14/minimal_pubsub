#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "broker.hpp"

using namespace pubsub;

TEST(BrokerTest, SubscribeAndPublish) {
    Broker<std::string> broker;
    auto ch = broker.subscribe();
    ASSERT_TRUE(ch);

    broker.publish(EventType::Created, "test");

    auto evt = ch->pop();
    ASSERT_TRUE(evt);
    EXPECT_EQ(evt->type, EventType::Created);
    EXPECT_EQ(evt->payload, "test");
}

TEST(BrokerTest, MultipleSubscribers) {
    Broker<int> broker;
    auto ch1 = broker.subscribe();
    auto ch2 = broker.subscribe();

    broker.publish(EventType::Updated, 42);

    auto evt1 = ch1->pop();
    auto evt2 = ch2->pop();
    ASSERT_TRUE(evt1);
    ASSERT_TRUE(evt2);
    EXPECT_EQ(evt1->payload, 42);
    EXPECT_EQ(evt2->payload, 42);
}

TEST(BrokerTest, Shutdown) {
    Broker<std::string> broker;
    auto ch = broker.subscribe();

    broker.shutdown();

    auto evt = ch->pop();
    EXPECT_FALSE(evt);  // Should return nullopt after shutdown
}

TEST(BrokerTest, SubscribeAfterShutdown) {
    Broker<std::string> broker;
    broker.shutdown();

    auto ch = broker.subscribe();
    EXPECT_FALSE(ch);
}