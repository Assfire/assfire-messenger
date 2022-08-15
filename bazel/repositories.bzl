load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def assfire_messenger_dependencies_kafka(repo_name):
    http_archive(
        name = "com_github_madler_zlib",
        strip_prefix = "zlib-1.2.12",
        build_file = repo_name + "//third_party:zlib.BUILD",
        url = "https://github.com/madler/zlib/archive/refs/tags/v1.2.12.tar.gz",
        sha256 = "d8688496ea40fb61787500e863cc63c9afcbc524468cedeb478068924eb54932",
    )

    http_archive(
        name = "com_github_edenhill_librdkafka",
        strip_prefix = "librdkafka-1.9.2",
        url = "https://github.com/edenhill/librdkafka/archive/refs/tags/v1.9.2.zip",
        build_file = repo_name + "//third_party:librdkafka.BUILD",
        sha256 = "4ecb0a3103022a7cab308e9fecd88237150901fa29980c99344218a84f497b86",
    )

    http_archive(
        name = "com_github_morganstanley_modern_cpp_kafka",
        strip_prefix = "modern-cpp-kafka-2022.06.15",
        url = "https://github.com/morganstanley/modern-cpp-kafka/archive/refs/tags/v2022.06.15.zip",
        sha256 = "3dc78cfccf4063579b18b726b02d4cf2f67ea1a01b3a46ead09b696cd0908ce3",
    )

    http_archive(
        name = "com_github_oneapi_src_onetbb",
        strip_prefix = "oneTBB-2021.5.0",
        url = "https://github.com/oneapi-src/oneTBB/archive/refs/tags/v2021.5.0.tar.gz",
        sha256 = "e5b57537c741400cf6134b428fc1689a649d7d38d9bb9c1b6d64f092ea28178a",
    )
