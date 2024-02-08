add_library(gymserver
  GymServer.cpp
  GymServer.h
)

# fmt
find_package(fmt CONFIG REQUIRED)
target_link_libraries(gymserver
PRIVATE
  sfml-network
  fmt::fmt
)