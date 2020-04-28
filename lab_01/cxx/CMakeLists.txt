cmake_minimum_required(VERSION 3.13)

set(SRC
    src/main.cc
)

add_executable(mmlab01
    ${SRC}
)

target_include_directories(mmlab01
    "PUBLIC"
        inc
        3rd_party
)

target_link_libraries(mmlab01
    mpfr
)

