execute_process(COMMAND "/home/mtrn4230/major-project-armee/ConnEGGt4_ws/build/pubsandsubs/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/mtrn4230/major-project-armee/ConnEGGt4_ws/build/pubsandsubs/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
