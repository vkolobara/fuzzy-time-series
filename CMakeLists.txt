cmake_minimum_required(VERSION 3.8)

project(fuzzy-time-series)
set(fuzzy-time-series_VERSION_MAJOR 1)
set(fuzzy-time-series_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/ClassifierFTSEvalOp.cpp src/ClassifierFTSEvalOp.h src/dataset/Dataset.cpp src/dataset/Dataset.h src/dataset/DataRow.cpp src/dataset/DataRow.h src/ErrorFunction.cpp src/ErrorFunction.h)

add_subdirectory(lib/ECF)
add_subdirectory(lib/fuzzy-inference-system)

add_executable(fts-regression src/main_regression.cpp ${SOURCE_FILES})
add_executable(fts-classification src/main_classification.cpp ${SOURCE_FILES} src/RegressionFTSEvalOp.cpp src/RegressionFTSEvalOp.h src/BestIndividualOperator.cpp src/BestIndividualOperator.h src/ANFISEvalOp.cpp src/ANFISEvalOp.h src/main_anfis.cpp)
add_executable(fts-financial src/main_financial.cpp ${SOURCE_FILES} src/FinTimeSeriesEvalOp.cpp src/FinTimeSeriesEvalOp.h)
add_executable(fts-anfis src/main_anfis.cpp ${SOURCE_FILES} src/ANFISEvalOp.h src/ANFISEvalOp.cpp)

target_include_directories(fts-regression PRIVATE lib/ECF)
target_include_directories(fts-regression PRIVATE lib/fuzzy-inference-system)
target_include_directories(fts-regression PRIVATE ${Boost_INCLUDE_DIR})

target_link_libraries(fts-regression ECF)
target_link_libraries(fts-regression FIS)

target_include_directories(fts-classification PRIVATE lib/ECF)
target_include_directories(fts-classification PRIVATE lib/fuzzy-inference-system)
target_include_directories(fts-classification PRIVATE ${Boost_INCLUDE_DIR})

target_link_libraries(fts-classification ECF)
target_link_libraries(fts-classification FIS)

target_include_directories(fts-financial PRIVATE lib/ECF)
target_include_directories(fts-financial PRIVATE lib/fuzzy-inference-system)
target_include_directories(fts-financial PRIVATE ${Boost_INCLUDE_DIR})

target_link_libraries(fts-financial ECF)
target_link_libraries(fts-financial FIS)

target_include_directories(fts-anfis PRIVATE lib/ECF)
target_include_directories(fts-anfis PRIVATE lib/fuzzy-inference-system)
target_include_directories(fts-anfis PRIVATE ${Boost_INCLUDE_DIR})

target_link_libraries(fts-anfis ECF)
target_link_libraries(fts-anfis FIS)
target_link_libraries(fts-anfis ANFIS)
