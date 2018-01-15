add_library(VecOps TVec.cxx)
target_link_libraries(VecOps ${ROOT_LIBRARIES})
target_include_directories(VecOps PUBLIC ${ROOT_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/inc)
