#include <iostream>
#include <thread>
#include <chrono>

#include <spdlog/spdlog.h>

#include "broker.hpp"

using namespace pubsub;

int main() {
    spdlog::set_level(spdlog::level::info);
    Broker<std::string> broker;

    // Subscribe
    auto ch = broker.subscribe();
    if (!ch) {
        std::cerr << "Failed to subscribe\n";
        return 1;
    }

    // Publish in a thread
    std::thread publisher([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        broker.publish(EventType::Created, "Hello");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        broker.publish(EventType::Updated, "World");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        broker.shutdown();
    });

    // Receive
    while (true) {
        auto evt = ch->pop();
        if (!evt) break;
        std::cout << "Received: " << static_cast<int>(evt->type) << " - " << evt->payload << std::endl;
    }

    publisher.join();
    std::cout << "Done\n";
    return 0;
}