#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "channel.hpp"
#include "event.hpp"

namespace pubsub{
     template <typename T>
     class Broker{
        public:
            using EventT = Event<T>;
            using ChannelT = Channel<Event<T>>;

            std::shared_ptr<ChannelT> subscribe();
            void publish(EventType type, T payload);
            void shutdown();

        private:
            std::mutex mu_;
            bool shutdown_ = false;
            std::size_t next_id_ = 1;
            std::unordered_map<std::size_t, std::shared_ptr<ChannelT>> subs_;
     };

     template <typename T>
     std::shared_ptr<typename Broker<T>::ChannelT> Broker<T>::subscribe() {
         std::lock_guard<std::mutex> lock(mu_);
         if (shutdown_) {
             spdlog::warn("Attempted to subscribe after shutdown");
             return nullptr;
         }
         auto ch = std::make_shared<ChannelT>();
         subs_[next_id_++] = ch;
         spdlog::info("New subscriber added, total: {}", subs_.size());
         return ch;
     }

     template <typename T>
     void Broker<T>::publish(EventType type, T payload) {
         std::lock_guard<std::mutex> lock(mu_);
         if (shutdown_) {
             spdlog::warn("Attempted to publish after shutdown");
             return;
         }
         EventT evt{type, payload};
         for (auto& [id, ch] : subs_) {
             ch->push(evt);
         }
         spdlog::info("Published event to {} subscribers", subs_.size());
     }

     template <typename T>
     void Broker<T>::shutdown() {
         std::lock_guard<std::mutex> lock(mu_);
         shutdown_ = true;
         for (auto& [id, ch] : subs_) {
             ch->close();
         }
         subs_.clear();
         spdlog::info("Broker shutdown, all channels closed");
     }
}