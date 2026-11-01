cmake_minimum_required(VERSION 3.8)
project(armando_description)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(trajectory_msgs REQUIRED)

# Compilazione del nodo di teleoperazione
add_executable(armando_teleop src/armando_teleop.cpp)
ament_target_dependencies(armando_teleop
  rclcpp
  trajectory_msgs
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Installazione dell'eseguibile nella cartella lib del pacchetto
install(
  TARGETS armando_teleop
  DESTINATION lib/${PROJECT_NAME}
)

# Installazione delle cartelle di risorse
install(
  DIRECTORY launch meshes urdf config worlds models
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
