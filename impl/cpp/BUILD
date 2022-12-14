cc_library(
    name = "assfire_messenger_cc_impl_kafka",
    srcs = [
        "assfire/messenger/impl/kafka/KafkaConsumer.cpp",
        "assfire/messenger/impl/kafka/KafkaMessageHeaders.cpp",
        "assfire/messenger/impl/kafka/KafkaMessenger.cpp",
        "assfire/messenger/impl/kafka/KafkaPublisher.cpp",
    ],
    hdrs = [
        "assfire/messenger/impl/kafka/KafkaConsumer.hpp",
        "assfire/messenger/impl/kafka/KafkaConsumerOptions.hpp",
        "assfire/messenger/impl/kafka/KafkaExceptions.hpp",
        "assfire/messenger/impl/kafka/KafkaMessageHeaders.hpp",
        "assfire/messenger/impl/kafka/KafkaMessenger.hpp",
        "assfire/messenger/impl/kafka/KafkaOptions.hpp",
        "assfire/messenger/impl/kafka/KafkaPublisher.hpp",
        "assfire/messenger/impl/kafka/KafkaPublisherOptions.hpp",
    ],
    includes = ["."],
    visibility = ["//visibility:public"],
    deps = [
        "//api/cpp:assfire_messenger_cc_api",
        "@com_github_assfire_assfire_logger//api/cpp:assfire_logger_cc_api",
        "@com_github_edenhill_librdkafka//:librdkafka",
        "@com_github_morganstanley_modern_cpp_kafka//:modern-cpp-kafka-api",
        "@com_github_oneapi_src_onetbb//:tbb",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "assfire_messenger_cc_impl_kafka_test",
    srcs = [
        "assfire/messenger/impl/kafka/test/KafkaMessageHeaders_Test.cpp",
        "assfire/messenger/impl/kafka/test/KafkaMessenger_Test.cpp",
    ],
    deps = [
        ":assfire_messenger_cc_impl_kafka",
        "@com_github_assfire_assfire_logger//impl/cpp:assfire_logger_cc_impl_spdlog",
        "@com_google_googletest//:gtest_main",
    ],
)
