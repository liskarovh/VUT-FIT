if(EXISTS "/home/hanka/Plocha/ivs_project_1/build/white_box_test[1]_tests.cmake")
  include("/home/hanka/Plocha/ivs_project_1/build/white_box_test[1]_tests.cmake")
else()
  add_test(white_box_test_NOT_BUILT white_box_test_NOT_BUILT)
endif()
