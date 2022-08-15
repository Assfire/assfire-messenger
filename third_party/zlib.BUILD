load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

licenses(["notice"])  # # Apache Kafka C driver library

filegroup(
    name = "zlib_srcs",
    srcs = glob(["**"]),
)

cmake(
    name = "zlib",
    includes = ["."],
    lib_source = ":zlib_srcs",
    out_static_libs = ["libz.a"],
    visibility = ["//visibility:public"],
)
