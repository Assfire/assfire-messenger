load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def assfire_messenger_dependencies_kafka(repo_name):
    http_archive(
        name = "com_github_edenhill_librdkafka",
        strip_prefix = "librdkafka-1.9.2",
        url = "https://github.com/edenhill/librdkafka/archive/refs/tags/v1.9.2.zip",
        build_file = "//third_party:librdkafka.BUILD",
        sha256 = "4ecb0a3103022a7cab308e9fecd88237150901fa29980c99344218a84f497b86",
    )

    http_archive(
        name = "com_github_morganstanley_modern_cpp_kafka",
        strip_prefix = "modern-cpp-kafka-2022.06.15",
        url = "https://github.com/morganstanley/modern-cpp-kafka/archive/refs/tags/v2022.06.15.zip",
        sha256 = "3dc78cfccf4063579b18b726b02d4cf2f67ea1a01b3a46ead09b696cd0908ce3",
    )
