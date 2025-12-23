#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

namespace pubsub{
    template <typename T>
    class Channel {
        public:
            void push(T v);
            std::optional<T> pop();
            void close();
        
        private:
            std::mutex mu_;
            std::condition_variable cv_;
            std::deque<T> q;
            bool closed_ = false;
    };

    template <typename T>
    void Channel<T>::push(T v) {
        std::lock_guard<std::mutex> lock(mu_);
        if (closed_) return;
        q.push_back(std::move(v));
        cv_.notify_one();
    }

    template <typename T>
    std::optional<T> Channel<T>::pop() {
        std::unique_lock<std::mutex> lock(mu_);
        cv_.wait(lock, [this] { return !q.empty() || closed_; });
        if (q.empty() && closed_) return std::nullopt;
        T val = std::move(q.front());
        q.pop_front();
        return val;
    }

    template <typename T>
    void Channel<T>::close() {
        std::lock_guard<std::mutex> lock(mu_);
        closed_ = true;
        cv_.notify_all();
    }
}  //namespace pubsub