#include "KafkaConsumer.hpp"

#include "KafkaMessageHeaders.hpp"
#include "assfire/messenger/api/Exceptions.hpp"

namespace assfire::messenger {

    KafkaConsumer::~KafkaConsumer() {
        stop();
    }

    KafkaConsumer::KafkaConsumer(std::shared_ptr<kafka::clients::KafkaConsumer> consumer) : _consumer(consumer) {}

    Message KafkaConsumer::poll() {
        while (true) {
            try {
                return poll(std::chrono::minutes(1));
            } catch (const TimeoutError& e) {
                // Just waiting for next loop
            }
        }
    }

    Message KafkaConsumer::poll(std::chrono::milliseconds timeout) {
        wait_for_new_messages(timeout);

        std::lock_guard<std::mutex> guard(_mtx);
        Message msg = _messages.front();
        _messages.pop();
        on_message_consumed();
        return msg;
    }

    void KafkaConsumer::ack(const Message& msg) {
        _consumer->commitSync({{kafka::TopicPartition(msg.headers().at(KAFKA_HEADER_TOPIC_NAME).value(),
                                                      std::stol(msg.headers().at(KAFKA_HEADER_TOPIC_PARTITION).value())),
                                msg.id()}});
    }

    void KafkaConsumer::stop() {
        _interrupted = true;
    }

    void KafkaConsumer::drain() {
        std::unique_lock<std::mutex> lck(_mtx);
        _drain_cv.wait(lck, [&] { return _messages.empty(); });
        lck.unlock();
    }

    void KafkaConsumer::consume_loop() {
        while (!_interrupted) {
            auto records = _consumer->poll(std::chrono::seconds(5));
            std::lock_guard guard(_mtx);
            for (const auto& record : records) {
                if (record.value().size() == 0) { continue; }
                if (!record.error()) {
                    Message msg(record.offset(), Payload(static_cast<const uint8_t*>(record.value().data()), record.value().size()));
                    msg.add_header(Header(KAFKA_HEADER_TOPIC_NAME, record.topic()));
                    msg.add_header(Header(KAFKA_HEADER_TOPIC_PARTITION, std::to_string(record.partition())));
                    _messages.emplace(std::move(msg));
                } else {
                    // Log message
                }
            }
            on_message_received();
        }
    }

    void KafkaConsumer::wait_for_new_messages(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lck(_mtx);
        if (!_poll_cv.wait_for(lck, timeout, [&] { return !_messages.empty(); })) {
            lck.unlock();
            throw TimeoutError();
        }
        lck.unlock();
    }

    void KafkaConsumer::on_message_received() {
        _poll_cv.notify_all();
    }

    void KafkaConsumer::on_message_consumed() {
        _drain_cv.notify_all();
    }

} // namespace assfire::messenger