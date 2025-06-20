cmake_minimum_required(VERSION 3.16)

project(C_game LANGUAGES C)

add_executable(C_game main.c)

include(GNUInstallDirs)
install(TARGETS C_game
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
