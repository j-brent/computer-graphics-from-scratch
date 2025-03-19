find_program(EMCC NAMES emcc NO_CACHE)

if(${EMCC} STREQUAL "EMCC-NOTFOUND")

  message("[CGFS] Couldn't find emscripten compiler. Skipping browser examples.")

else()

  add_executable(browser-example browser-example.cpp)
  target_link_libraries(browser-example cgfs::graphics)
  add_custom_command(
        TARGET browser-example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/browser-example.html
                ${CMAKE_CURRENT_BINARY_DIR}/browser-example.html)

  add_executable(browser-example-animation browser-example-animation.cpp)
  target_link_libraries(browser-example-animation cgfs::graphics)
  add_custom_command(
        TARGET browser-example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/browser-example-animation.html
                ${CMAKE_CURRENT_BINARY_DIR}/browser-example-animation.html)

endif()
