#include "KafkaConsumer.hpp"

#include "KafkaMessageHeaders.hpp"
#include "assfire/logger/api/LoggerProvider.hpp"
#include "assfire/messenger/api/Exceptions.hpp"

namespace assfire::messenger {

    KafkaConsumer::~KafkaConsumer() {
        stop();
    }

    KafkaConsumer::KafkaConsumer(std::shared_ptr<kafka::clients::KafkaConsumer> consumer, KafkaConsumerOptions options)
        : _consumer(consumer),
          _interrupted(false),
          _started(false),
          _consumer_options(options),
          _logger(logger::LoggerProvider::get("assfire.messenger.KafkaConsumer")) {}

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

        Message msg;
        if (!_messages.try_pop(msg)) { throw EndOfStreamError(); }
        on_message_consumed();
        return msg;
    }

    void KafkaConsumer::ack(const Message& msg) {
        try {
            _consumer->commitSync(
                {{kafka::TopicPartition(*msg.header(KAFKA_HEADER_TOPIC_NAME), decode_partition_header(*msg.header(KAFKA_HEADER_TOPIC_PARTITION))),
                  decode_offset_header(*msg.header(KAFKA_HEADER_OFFSET))}});
        } catch (const std::exception& e) {
            std::string headers_string = msg.headers_to_string();
            _logger->error("Failed to ack message with headers {}: {}", headers_string, e.what());
            std::throw_with_nested(AckFailedError("Failed to ack message: " + headers_string));
        }
    }

    void KafkaConsumer::pause() {
        _consumer->pause();
    }

    void KafkaConsumer::resume() {
        _consumer->resume();
    }

    void KafkaConsumer::stop() {
        _interrupted = true;
    }

    void KafkaConsumer::drain() {
        std::unique_lock<std::mutex> lck(_drain_mtx);
        _drain_cv.wait(lck, [&] { return _messages.empty(); });
    }

    const KafkaConsumerOptions& KafkaConsumer::options() {
        return _consumer_options;
    }

    void KafkaConsumer::consume_loop() {
        while (!_interrupted) {
            auto records = _consumer->poll(std::chrono::seconds(5));
            for (const auto& record : records) {
                if (record.value().size() == 0) { continue; }
                if (!record.error()) {
                    Message msg(Payload(static_cast<const uint8_t*>(record.value().data()), record.value().size()));
                    msg.add_header(Header(KAFKA_HEADER_OFFSET, encode_offset_header(record.offset())));
                    msg.add_header(Header(KAFKA_HEADER_TOPIC_NAME, record.topic()));
                    msg.add_header(Header(KAFKA_HEADER_TOPIC_PARTITION, encode_partition_header(record.partition())));
                    _messages.emplace(std::move(msg));
                } else {
                    // Log message
                }
            }
            on_message_received();
        }
    }

    void KafkaConsumer::wait_for_new_messages(std::chrono::milliseconds timeout) {
        if (!_started) {
            bool expected_started = false;
            if (_started.compare_exchange_strong(expected_started, true)) {
                _work_ftr = std::async(std::launch::async, std::bind(&KafkaConsumer::consume_loop, this));
            }
        }
        std::unique_lock<std::mutex> lck(_poll_mtx);
        if (!_poll_cv.wait_for(lck, timeout, [&] { return !_messages.empty(); })) { throw TimeoutError(); }
    }

    void KafkaConsumer::on_message_received() {
        _poll_cv.notify_all();
    }

    void KafkaConsumer::on_message_consumed() {
        _drain_cv.notify_all();
    }

} // namespace assfire::messenger