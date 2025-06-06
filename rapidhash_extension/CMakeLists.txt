cmake_minimum_required(VERSION 2.8.12...3.29)

project(RapidHashExtension)

# Configure extension
set(EXTENSION_NAME rapidhash_extension)

set(EXTENSION_FILES rapidhash_extension.cpp)

option(
  USE_UNSTABLE_C_API
  "Use the unstable C Extension API (extension will be tied to exact DuckDB version)"
  TRUE)

if(USE_UNSTABLE_C_API)
  build_loadable_extension_capi_unstable(${EXTENSION_NAME} ${EXTENSION_FILES})
else()
  # Minimum supported DuckDB version
  set(CAPI_MAJOR 1)
  set(CAPI_MINOR 3)
  set(CAPI_PATCH 0)
  build_loadable_extension_capi(${EXTENSION_NAME} ${CAPI_MAJOR} ${CAPI_MINOR}
                                ${CAPI_PATCH} ${EXTENSION_FILES})
endif()