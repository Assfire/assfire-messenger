#pragma once

#include "assfire/messenger/api/Consumer.hpp"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <kafka/KafkaConsumer.h>
#include <memory>
#include <mutex>
#include <oneapi/tbb/concurrent_queue.h>

namespace assfire::messenger {
    class KafkaConsumer : public Consumer {
      public:
        ~KafkaConsumer();

        KafkaConsumer(std::shared_ptr<kafka::clients::KafkaConsumer> consumer);
        virtual Message poll() override;
        virtual Message poll(std::chrono::milliseconds timeout) override;
        virtual void ack(const Message& msg) override;
        virtual void pause() override;
        virtual void resume() override;
        virtual void stop() override;
        virtual void drain() override;

      private:
        void on_message_received();
        void on_message_consumed();
        void wait_for_new_messages(std::chrono::milliseconds timeout);
        void consume_loop();

        std::shared_ptr<kafka::clients::KafkaConsumer> _consumer;
        std::mutex _poll_mtx;
        std::mutex _drain_mtx;
        std::condition_variable _poll_cv;
        std::condition_variable _drain_cv;
        std::future<void> _work_ftr;
        tbb::concurrent_queue<Message> _messages;
        std::atomic_bool _interrupted;
    };
} // namespace assfire::messenger