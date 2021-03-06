# Copyright (c) 2018-2019, CNRS-UM LIRMM
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

set(CONTROLLER_SRC
    Controller.cpp
    FloatingBaseObserver.cpp
    FootstepPlan.cpp
    ModelPredictiveControl.cpp
    NetWrenchObserver.cpp
    Pendulum.cpp
    Stabilizer.cpp
    SwingFoot.cpp)

set(CONTROLLER_HDR
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/Contact.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/Controller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/FloatingBaseObserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/FootstepPlan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/ModelPredictiveControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/NetWrenchObserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/Pendulum.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/Preview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/Sole.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/Stabilizer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/State.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/SwingFoot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/defs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/utils/ExponentialMovingAverage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/utils/LeakyIntegrator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/utils/LowPassVelocityFilter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/utils/StationaryOffsetFilter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/utils/clamp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/utils/polynomials.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/lipm_walking/utils/slerp.h)

set(CONTROLLER_NAME "${PROJET_NAME}_controller")

add_library(${PROJECT_NAME} SHARED ${CONTROLLER_SRC} ${CONTROLLER_HDR})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DMC_CONTROL_EXPORTS")
add_dependencies(${PROJECT_NAME} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} copra::copra eigen-lssol::eigen-lssol mc_rtc::mc_control_fsm)
install(TARGETS ${PROJECT_NAME} DESTINATION ${MC_RTC_LIBDIR}/mc_controller)

add_library(${CONTROLLER_NAME} SHARED lib.cpp)
set_target_properties(${CONTROLLER_NAME}
  PROPERTIES
    PREFIX ""
    OUTPUT_NAME "${PROJECT_NAME}"
    LIBRARY_OUTPUT_DIRECTORY "${CATKIN_DEVEL_PREFIX}/lib/${PROJECT_NAME}")
target_link_libraries(${CONTROLLER_NAME} ${PROJECT_NAME})
install(TARGETS ${CONTROLLER_NAME} DESTINATION ${MC_RTC_LIBDIR}/mc_controller)

set(CONF_OUT "$ENV{HOME}/.config/mc_rtc/controllers/LIPMWalking.conf")
set(AROBASE "@")
set(LIPM_WALKING_STATES_DIR "${CATKIN_DEVEL_PREFIX}/lib/${PROJECT_NAME}/states/")
set(LIPM_WALKING_STATES_DATA_DIR "${LIPM_WALKING_STATES_DIR}/data")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../etc/LIPMWalking.conf.cmake" "${CONF_OUT}")
unset(AROBASE)

set(LIPM_WALKING_STATES_DIR "${MC_RTC_LIBDIR}/mc_controller/${PROJECT_NAME}/states")
set(LIPM_WALKING_STATES_DATA_DIR "${LIPM_WALKING_STATES_DIR}/data")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../etc/LIPMWalking.conf.cmake" "${CMAKE_CURRENT_BINARY_DIR}/etc/LIPMWalking.conf")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/etc/LIPMWalking.conf"
    DESTINATION "${MC_RTC_LIBDIR}/mc_controller/etc/")

add_subdirectory(states)
