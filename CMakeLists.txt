  cmake_minimum_required(VERSION 2.8)

  set(PORT /dev/ttyACM0 CACHE STRING "uploading serial port")
  set(IS_NATIVE_PORT true CACHE STRING "is it the native port? (true), or not? (false)")

  set(CMAKE_TOOLCHAIN_FILE arduino_due_toolchain.cmake)

  project(pwm_lib C CXX)

  #####################################################################
  # generating a static library for the standard Arduino library: begin
  #####################################################################

  file(
    GLOB ARDUINO_DUE_STD_LIB_SOURCE_FILES
    LIST_DIRECTORIES false 
    $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/cores/arduino/*.c
    $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/cores/arduino/*.cpp
    $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/cores/arduino/USB/*.c
    $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/cores/arduino/USB/*.cpp
    $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/variants/arduino_due_x/*.c
    $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/variants/arduino_due_x/*.cpp
    $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/libraries/SPI/*.c
    $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/libraries/SPI/*.cpp
  )

  #mess(STATUS "ARDUINO_DUE_STD_LIB_SOURCE_FILES: ${ARDUINO_DUE_STD_LIB_SOURCE_FILES}")

  add_library(arduino_due_std_lib STATIC ${ARDUINO_DUE_STD_LIB_SOURCE_FILES})
  set_property(
    TARGET arduino_due_std_lib PROPERTY 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
  target_compile_options(
    arduino_due_std_lib PUBLIC 
    -c -g -Os -w -ffunction-sections -fdata-sections -nostdlib -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -MMD -mcpu=cortex-m3 -std=gnu++11 -mthumb
  )

  target_compile_definitions(
    arduino_due_std_lib PUBLIC
    -Dprintf=iprintf -DF_CPU=84000000L -DARDUINO=10605 -DARDUINO_SAM_DUE -DARDUINO_ARCH_SAM -D__SAM3X8E__ -DUSB_VID=0x2341 -DUSB_PID=0x003e -DUSBCON -DUSB_MANUFACTURER="Unknown" -DUSB_PRODUCT="Arduino Due"
  )
  #####################################################################
  # generating a static library for the standard Arduino library: end 
  #####################################################################

  # checking environment variable ARDUINO_IDE_LIBRARY_PATH
  if( 
    NOT (DEFINED ENV{ARDUINO_IDE_LIBRARY_PATH})
    OR 
    ($ENV{ARDUINO_IDE_LIBRARY_PATH} EQUAL "")
  )

    message(FATAL_ERROR "[ERROR] Environment variable ARDUINO_IDE_LIBRARY_PATH not set!")

  else()

    message(STATUS "Environment variable ARDUINO_IDE_LIBRARY_PATH: $ENV{ARDUINO_IDE_LIBRARY_PATH}")

  endif()

  #####################################################################
  # generating a static library for pwm_lib: begin
  #####################################################################

  file(
    GLOB PWM_LIB_SOURCE_FILES
    LIST_DIRECTORIES false 
    $ENV{ARDUINO_IDE_LIBRARY_PATH}/libraries/pwm_lib/*.c
    $ENV{ARDUINO_IDE_LIBRARY_PATH}/libraries/pwm_lib/*.cpp
  )

  message(STATUS "PWM_LIB_SOURCE_FILES: ${PWM_LIB_SOURCE_FILES}")

  add_library(pwm_lib STATIC ${PWM_LIB_SOURCE_FILES})
  set_property(
    TARGET pwm_lib PROPERTY 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
  target_compile_options(
    pwm_lib PUBLIC 
    -c -g -Os -w -ffunction-sections -fdata-sections -nostdlib -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -MMD -mcpu=cortex-m3 -std=gnu++11 -mthumb
  )

  target_compile_definitions(
    pwm_lib PUBLIC
    -Dprintf=iprintf -DF_CPU=84000000L -DARDUINO=10605 -DARDUINO_SAM_DUE -DARDUINO_ARCH_SAM -D__SAM3X8E__ -DUSB_VID=0x2341 -DUSB_PID=0x003e -DUSBCON -DUSB_MANUFACTURER="Unknown" -DUSB_PRODUCT="Arduino Due"
  )

  target_include_directories(
    pwm_lib PUBLIC
    $ENV{ARDUINO_IDE_LIBRARY_PATH}/libraries/pwm_lib/
  )
  #####################################################################
  # generating a static library for pwm_lib: end 
  #####################################################################

  #####################################################################
  # pwm_lib examples: begin 
  #####################################################################

  set(
    TC_EXAMPLES
    basic_test
    servo_test
  )

  foreach(src_example ${TC_EXAMPLES})
    configure_file(
      ${PROJECT_SOURCE_DIR}/examples/${src_example}/${src_example}.ino
      ${src_example}.cpp
      COPYONLY
    )
    add_executable(
      ${src_example}.cpp.elf 
      ${src_example}.cpp
      $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/cores/arduino/syscalls_sam3.c
    )
    target_compile_options(
      ${src_example}.cpp.elf PUBLIC 
      -c -g -Os -w -ffunction-sections -fdata-sections -nostdlib -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -MMD -mcpu=cortex-m3 -std=gnu++11 -mthumb
    )
    
    target_compile_definitions(
      ${src_example}.cpp.elf PUBLIC
      -Dprintf=iprintf -DF_CPU=84000000L -DARDUINO=10605 -DARDUINO_SAM_DUE -DARDUINO_ARCH_SAM -D__SAM3X8E__ -DUSB_VID=0x2341 -DUSB_PID=0x003e -DUSBCON -DUSB_MANUFACTURER="Unknown" -DUSB_PRODUCT="Arduino Due"
    )
    
    target_include_directories(
      ${src_example}.cpp.elf PUBLIC
      $ENV{ARDUINO_IDE_LIBRARY_PATH}/libraries/pwm_lib
      $ENV{ARDUINO_IDE_LIBRARY_PATH}/libraries/tc_lib
    )
    target_link_libraries(
      ${src_example}.cpp.elf
      -Os -Wl,--gc-sections -mcpu=cortex-m3 -T$ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/variants/arduino_due_x/linker_scripts/gcc/flash.ld -Wl,-Map,${src_example}.cpp.map -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols -Wl,--start-group arduino_due_std_lib pwm_lib $ENV{ARDUINO_DUE_ROOT_PATH}/hardware/sam/1.6.6/variants/arduino_due_x/libsam_sam3x8e_gcc_rel.a -Wl,--end-group -lm 
    )
    
    # bin
    add_custom_command(
      OUTPUT
	${src_example}.cpp.bin 
      COMMAND 
	${DUE_OBJCOPY} -O binary ${src_example}.cpp.elf ${src_example}.cpp.bin 
    DEPENDS
      ${src_example}.cpp.elf
  )
  
  # size
  add_custom_command(
    OUTPUT
      ${src_example}.cpp.size
    COMMAND
      ${DUE_SIZE_TOOL} -A ${src_example}.cpp.elf > ${src_example}.cpp.size
    DEPENDS
      ${src_example}.cpp.elf
  )
  
  # target for the custom commands
  add_custom_target(
    ${src_example}  
    ALL
    DEPENDS
      ${src_example}.cpp.size ${src_example}.cpp.bin 
  )
  
  # upload - with avrdude
  get_filename_component(PORT_NAME ${PORT} NAME)
  add_custom_target(
    upload_${src_example} 
    COMMAND
      stty -F ${PORT} 1200\; cat ${CMAKE_CURRENT_LIST_DIR}/dummy > ${PORT}\; sleep 3 
    COMMAND
      ${DUE_BOSSAC} -i -d --port=${PORT_NAME} -U ${IS_NATIVE_PORT} -e -w -v -b ${src_example}.cpp.bin -R 
    DEPENDS ${src_example}.cpp.bin
    COMMENT 
      "Uploading ${src_example}.cpp.bin to uC ATSAM3X8E through ${PORT}" 
  ) 
  
endforeach(src_example)





