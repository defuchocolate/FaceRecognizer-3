INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCE
   ${CMAKE_CURRENT_SOURCE_DIR}/GPIOTrigger.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/FaceWrapper.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/FaceDetector.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/FaceRecognizer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

ADD_EXECUTABLE(facereg ${SOURCE})
SET_TARGET_PROPERTIES(facereg PROPERTIES LINK_FLAGS "-lpthread -lopencv_core -lopencv_highgui -lopencv_contrib -lopencv_objdetect -lopencv_imgproc")
