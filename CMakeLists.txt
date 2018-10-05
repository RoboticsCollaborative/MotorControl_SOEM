
set(SOURCES config_SDOparam.c haptic_config.c haptic_run.c)
add_executable(haptic_run ${SOURCES})
target_link_libraries(haptic_run soem)
install(TARGETS haptic_run DESTINATION bin)
