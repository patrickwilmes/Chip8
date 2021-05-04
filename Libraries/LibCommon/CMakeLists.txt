set(SOURCES
        Types.h
        Types.cpp
        Assert.cpp
        Assert.h
        Print.h
        Print.cpp
        Path.h
        Path.cpp
        User.h
        User.cpp
        )

add_library(LibCommon ${SOURCES})
target_include_directories(LibCommon PUBLIC .)