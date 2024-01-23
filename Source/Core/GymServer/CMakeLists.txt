add_library(gymserver
  GymServer.cpp
  GymServer.h
)

target_link_libraries(gymserver
PRIVATE
  sfml-network
)