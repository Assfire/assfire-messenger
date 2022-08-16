cc_library(
    name = "assfire_messenger_cc_api",
    srcs = [
        "assfire/messenger/api/Api.cpp",
        "assfire/messenger/api/Payload.cpp",
    ],
    hdrs = [
        "assfire/messenger/api/ChannelId.hpp",
        "assfire/messenger/api/Consumer.hpp",
        "assfire/messenger/api/Exceptions.hpp",
        "assfire/messenger/api/Header.hpp",
        "assfire/messenger/api/Message.hpp",
        "assfire/messenger/api/Messenger.hpp",
        "assfire/messenger/api/Payload.hpp",
        "assfire/messenger/api/Publisher.hpp",
    ],
    includes = ["."],
    visibility = ["//visibility:public"],
    deps = ["@com_google_absl//absl/strings"],
)
