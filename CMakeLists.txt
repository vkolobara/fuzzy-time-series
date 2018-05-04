cmake_minimum_required(VERSION 3.8)

project(fuzzy-time-series)
set(fuzzy-time-series_VERSION_MAJOR 1)
set(fuzzy-time-series_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp
        src/ClassifierFTSEvalOp.cpp src/ClassifierFTSEvalOp.h src/dataset/Dataset.cpp src/dataset/Dataset.h src/dataset/DataRow.cpp src/dataset/DataRow.h src/BestIndividualOperator.cpp src/BestIndividualOperator.h src/RegressionFTSEvalOp.cpp src/RegressionFTSEvalOp.h)

add_subdirectory(lib/ECF)
add_subdirectory(lib/fuzzy-inference-system)

add_executable(fuzzy-time-series ${SOURCE_FILES})

target_include_directories(fuzzy-time-series PRIVATE lib/ECF)
target_include_directories(fuzzy-time-series PRIVATE lib/fuzzy-inference-system)
target_include_directories(fuzzy-time-series PRIVATE ${Boost_INCLUDE_DIR})

target_link_libraries(fuzzy-time-series ECF)
target_link_libraries(fuzzy-time-series FIS)

