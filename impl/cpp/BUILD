cc_library(
    name = "assfire_messenger_cc_impl_kafka",
    srcs = ["assfire/messenger/impl/kafka/KafkaMessenger.cpp"],
    hdrs = [
        "assfire/messenger/impl/kafka/KafkaConsumer.hpp",
        "assfire/messenger/impl/kafka/KafkaMessenger.hpp",
    ],
    deps = [
        "//api/cpp:assfire_messenger_cc_api",
        "@com_github_edenhill_librdkafka//:librdkafka",
        "@com_github_morganstanley_modern_cpp_kafka//:modern-cpp-kafka-api",
    ],
)
