filegroup(
    name = "example_files",
    srcs = glob(["example_*.txt"]),
    data = glob(["example_*.txt"]),
)

cc_binary(
    name = "day_04",
    srcs = ["main.cpp"],
    deps = [
        "//common:aoc_day",
        "//common:utils",
    ],
    data = [
        "input.txt",
        ":example_files",
    ],
)
