cmake_minimum_required(VERSION 3.8)

project(fuzzy-time-series)
set(fuzzy-time-series_VERSION_MAJOR 1)
set(fuzzy-time-series_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/classification/ClassifierFTSEvalOp.cpp
        src/classification/ClassifierFTSEvalOp.h src/dataset/Dataset.cpp
        src/dataset/Dataset.h src/dataset/DataRow.cpp src/dataset/DataRow.h
        src/ErrorFunction.cpp src/ErrorFunction.h src/FileLogger.cpp
        src/FileLogger.h)

add_subdirectory(lib/ECF)
add_subdirectory(lib/fuzzy-inference-system)

add_executable(fts-regression
        src/regression/RegressionFTSEvalOp.cpp
        src/regression/RegressionFTSEvalOp.h
        src/regression/Regression2FTSEvalOp.cpp
        src/regression/Regression2FTSEvalOp.h
        src/regression/RegressionLoggingOperator.cpp
        src/regression/RegressionLoggingOperator.h
        src/regression/main_regression.cpp ${SOURCE_FILES} src/regression/RegressionRules.cpp src/regression/RegressionRules.h)

add_executable(fts-classification src/classification/main_classification.cpp
        src/classification/ClassificationLoggingOperator.cpp
        src/classification/ClassificationLoggingOperator.h ${SOURCE_FILES})

add_executable(fts-financial src/financial/main_financial.cpp
        src/financial/FinTimeSeriesEvalOp.cpp src/financial/FinTimeSeriesEvalOp.h
        src/financial/FinancialLoggingOperator.cpp
        src/financial/FinancialLoggingOperator.h ${SOURCE_FILES})

add_executable(fts-anfis src/anfis/main_anfis.cpp src/anfis/ANFISEvalOp.cpp src/anfis/ANFISEvalOp.h src/anfis/main_anfis.cpp
        ${SOURCE_FILES})

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
