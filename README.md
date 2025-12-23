# PubSubCpp

A minimal, thread-safe publish-subscribe (pub/sub) library implemented in C++20. This project demonstrates event-driven programming with generics, concurrency, and modern C++ features. 

## Features

- **Generic Payloads**: Supports any data type via templates.
- **Thread-Safe**: Uses mutexes and condition variables for concurrent access.
- **Blocking Channels**: Subscribers wait for events or shutdown.
- **Simple API**: Easy to subscribe, publish, and shutdown.
- **Logging**: Integrated with spdlog for observability.
- **Unit Tests**: Comprehensive tests with Google Test.

## Architecture

- `Event<T>`: Represents an event with type and payload.
- `Channel<T>`: Thread-safe queue for events.
- `Broker<T>`: Manages subscriptions and publishing.

## Build Instructions

Requires C++20 and CMake 3.20+.

```bash
mkdir build
cd build
cmake ..
make
```

## Usage Example

```cpp
#include "broker.hpp"
#include <spdlog/spdlog.h>

using namespace pubsub;

int main() {
    spdlog::set_level(spdlog::level::info);
    Broker<std::string> broker;

    auto channel = broker.subscribe();
    broker.publish(EventType::Created, "Hello World");

    auto event = channel->pop();
    if (event) {
        std::cout << "Received: " << event->payload << std::endl;
    }

    broker.shutdown();
    return 0;
}
```

## Running Tests

```bash
cd build
ctest
```


