# --------------------------------------------------------------
#  core
# --------------------------------------------------------------
set(target "${MODULE_NAME}_more")
file(GLOB CORE_SOURCES dub/*.cpp *.cpp ${PLAT}/*.cpp ${PLAT}/*.mm)
add_library(${target} MODULE ${CORE_SOURCES})
set_target_properties(${target}
  PROPERTIES OUTPUT_NAME more_c
  LINK_FLAGS ${PLAT_OBJ_FLAGS}
  PREFIX ""
  SUFFIX ".so"
)  

add_library(${target}_static ${CORE_SOURCES})

# msgpack needed for lk.Socket
# time_ref needed by lk.TimeRef
target_link_libraries(${target} msgpack time_ref lua_dump)
target_link_libraries(${target}_static msgpack time_ref lua_dump)

add_dependencies(${MODULE_NAME} ${target})

# --------------------------------------------------------------
#  install                                     
# --------------------------------------------------------------
install(TARGETS ${target}
  DESTINATION ${LUBYK_LIB}/lib/${MODULE_NAME}
)
