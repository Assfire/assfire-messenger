load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

licenses(["notice"])  # # Apache Kafka C driver library

filegroup(
    name = "librdkafka_srcs",
    srcs = glob(["**"]),
)

cmake(
    name = "librdkafka",
    includes = ["include"],
    lib_source = ":librdkafka_srcs",
    out_shared_libs = [
        "librdkafka.so",
        "librdkafka++.so",
    ],
    visibility = ["//visibility:public"],
)
