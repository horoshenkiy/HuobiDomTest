# CMakeLists.txt: Add source to this project's executable.
#
file(GLOB_RECURSE HUOBI_DOM_TEST_SRC ./*.cpp)
add_executable (HuobiDomTest ${HUOBI_DOM_TEST_SRC})

if (MINGW)
    target_link_options(HuobiDomTest PRIVATE -static-libstdc++ -static-libgcc)
endif()

# TODO: Add tests and install targets if needed.
