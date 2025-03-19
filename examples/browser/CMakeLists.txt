find_program(EMCC NAMES emcc NO_CACHE)

if(${EMCC} STREQUAL "EMCC-NOTFOUND")
  message("[CGFS] Couldn't find emscripten compiler. Skipping browser example.")
else()
  add_executable(browser-example main.cpp)
  target_link_libraries(browser-example cgfs::graphics)
  add_custom_command(
        TARGET browser-example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/browser-example.html
                ${CMAKE_CURRENT_BINARY_DIR}/browser-example.html)
endif()
