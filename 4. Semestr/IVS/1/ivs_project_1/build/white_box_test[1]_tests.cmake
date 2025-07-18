add_test( EmptyTable.CreateAndDestroy /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=EmptyTable.CreateAndDestroy]==] --gtest_also_run_disabled_tests)
set_tests_properties( EmptyTable.CreateAndDestroy PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( EmptyTable.InsertSingleElement /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=EmptyTable.InsertSingleElement]==] --gtest_also_run_disabled_tests)
set_tests_properties( EmptyTable.InsertSingleElement PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( EmptyTable.RetrieveFromEmpty /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=EmptyTable.RetrieveFromEmpty]==] --gtest_also_run_disabled_tests)
set_tests_properties( EmptyTable.RetrieveFromEmpty PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( EmptyTable.RemoveFromEmpty /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=EmptyTable.RemoveFromEmpty]==] --gtest_also_run_disabled_tests)
set_tests_properties( EmptyTable.RemoveFromEmpty PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( EmptyTable.ContainsEmpty /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=EmptyTable.ContainsEmpty]==] --gtest_also_run_disabled_tests)
set_tests_properties( EmptyTable.ContainsEmpty PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( NonEmptyTable.InsertAndRetrieve /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=NonEmptyTable.InsertAndRetrieve]==] --gtest_also_run_disabled_tests)
set_tests_properties( NonEmptyTable.InsertAndRetrieve PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( NonEmptyTable.UpdateExistingKey /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=NonEmptyTable.UpdateExistingKey]==] --gtest_also_run_disabled_tests)
set_tests_properties( NonEmptyTable.UpdateExistingKey PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( NonEmptyTable.RemoveKey /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=NonEmptyTable.RemoveKey]==] --gtest_also_run_disabled_tests)
set_tests_properties( NonEmptyTable.RemoveKey PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( NonEmptyTable.ContainsKey /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=NonEmptyTable.ContainsKey]==] --gtest_also_run_disabled_tests)
set_tests_properties( NonEmptyTable.ContainsKey PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( NonEmptyTable.HandleCollisions /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=NonEmptyTable.HandleCollisions]==] --gtest_also_run_disabled_tests)
set_tests_properties( NonEmptyTable.HandleCollisions PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( NonEmptyTable.ClearTable /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=NonEmptyTable.ClearTable]==] --gtest_also_run_disabled_tests)
set_tests_properties( NonEmptyTable.ClearTable PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( NonEmptyTable.PopElements /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=NonEmptyTable.PopElements]==] --gtest_also_run_disabled_tests)
set_tests_properties( NonEmptyTable.PopElements PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test( NonEmptyTable.ResizeTable /home/hanka/Plocha/ivs_project_1/build/white_box_test [==[--gtest_filter=NonEmptyTable.ResizeTable]==] --gtest_also_run_disabled_tests)
set_tests_properties( NonEmptyTable.ResizeTable PROPERTIES WORKING_DIRECTORY /home/hanka/Plocha/ivs_project_1/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( white_box_test_TESTS EmptyTable.CreateAndDestroy EmptyTable.InsertSingleElement EmptyTable.RetrieveFromEmpty EmptyTable.RemoveFromEmpty EmptyTable.ContainsEmpty NonEmptyTable.InsertAndRetrieve NonEmptyTable.UpdateExistingKey NonEmptyTable.RemoveKey NonEmptyTable.ContainsKey NonEmptyTable.HandleCollisions NonEmptyTable.ClearTable NonEmptyTable.PopElements NonEmptyTable.ResizeTable)
