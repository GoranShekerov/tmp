

cmake_minimum_required (VERSION 3.21)

project(ExampleLivio)         



set(CMAKE_CXX_STANDARD 17)

add_executable(OpenClosedExerciseLivio 
    OCPExercise-Solution.cpp    
    OCPDependencies.h
)

#set_target_properties(OpenClosedExerciseLivio PROPERTIES LINKER_LANGUAGE CXX)