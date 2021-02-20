cmake_minimum_required(VERSION 3.5)
project(litejvm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(INCLUDE_LIST
        include/arrayobject.h        
        include/classinstance.h
        include/classloader.h
        include/classruntime.h
        include/classviewer.h
        include/vmengine.h
        include/frame.h
        include/heap.h
        include/object.h
       include/stringobject.h
       include/tipos.h
        include/utils.h
        include/vmstack.h
       )
set(SRC_LIST
        src/arrayobject.cpp
        src/arrayobject.cpp
        src/stringobject.cpp
        src/classinstance.cpp  
       src/classloader.cpp      
       src/vmengine.cpp  
       src/main.cpp        
       src/classruntime.cpp     
       src/frame.cpp            
       src/methodarea.cpp 
       src/utils.cpp
        src/classviewer.cpp    
        src/heap.cpp          
        src/vmstack.cpp
         )

include_directories(include)
 

add_definitions(-Og -g)

add_executable(litejvm  ${INCLUDE_LIST} ${SRC_LIST})
 
