set(LIB_NAME "Sparrow-Maths")

add_library(${LIB_NAME} STATIC
        Hello.cpp
        Hello.h
)

# Setup Headers
target_include_directories(${LIB_NAME}
        PUBLIC
        "./"
)