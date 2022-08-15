load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

licenses(["notice"])  # # Apache Kafka C driver library

filegroup(
    name = "librdkafka_srcs",
    srcs = glob(["**"]),
)

cmake(
    name = "librdkafka",
    generate_args = ["-DRDKAFKA_BUILD_STATIC=True"],
    includes = ["include"],
    lib_source = ":librdkafka_srcs",
    visibility = ["//visibility:public"],
    deps = ["@com_github_madler_zlib//:zlib"],
)
