add_library(SolverTest STATIC testsolver.cpp)
target_include_directories(SolverTest PUBLIC include)

target_link_libraries(SolverTest PUBLIC SolverInterface CommonInclude)
target_link_libraries(SolverTest PRIVATE errorhandling)


