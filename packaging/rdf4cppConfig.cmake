cmake_minimum_required(VERSION 3.19)

set(rdf2cpp_known_comps static shared)
set(rdf2cpp_comp_static NO)
set(rdf2cpp_comp_shared NO)
foreach (rdf2cpp_comp IN LISTS ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
    if (rdf2cpp_comp IN_LIST rdf2cpp_known_comps)
        set(rdf2cpp_comp_${rdf2cpp_comp} YES)
    else ()
        set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
            "rdf2cpp does not recognize component `${rdf2cpp_comp}`.")
        set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
        return()
    endif ()
endforeach ()

if (rdf2cpp_comp_static AND rdf2cpp_comp_shared)
    set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
        "rdf2cpp `static` and `shared` components are mutually exclusive.")
    set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
    return()
endif ()

set(rdf2cpp_static_targets "${CMAKE_CURRENT_LIST_DIR}/rdf2cpp-static-targets.cmake")
set(rdf2cpp_shared_targets "${CMAKE_CURRENT_LIST_DIR}/rdf2cpp-shared-targets.cmake")

macro(rdf2cpp_load_targets type)
    if (NOT EXISTS "${rdf2cpp_${type}_targets}")
        set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
            "rdf2cpp `${type}` libraries were requested but not found.")
        set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
        return()
    endif ()
    include("${rdf2cpp_${type}_targets}")
endmacro()

if (rdf2cpp_comp_static)
    rdf2cpp_load_targets(static)
elseif (rdf2cpp_comp_shared)
    rdf2cpp_load_targets(shared)
elseif (DEFINED rdf2cpp_SHARED_LIBS AND rdf2cpp_SHARED_LIBS)
    rdf2cpp_load_targets(shared)
elseif (DEFINED rdf2cpp_SHARED_LIBS AND NOT rdf2cpp_SHARED_LIBS)
    rdf2cpp_load_targets(static)
elseif (BUILD_SHARED_LIBS)
    if (EXISTS "${rdf2cpp_shared_targets}")
        rdf2cpp_load_targets(shared)
    else ()
        rdf2cpp_load_targets(static)
    endif ()
else ()
    if (EXISTS "${rdf2cpp_static_targets}")
        rdf2cpp_load_targets(static)
    else ()
        rdf2cpp_load_targets(shared)
    endif ()
endif ()
