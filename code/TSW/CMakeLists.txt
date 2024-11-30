cmake_minimum_required(VERSION 3.16)
project(SW_BFC)

set(CMAKE_CXX_STANDARD 14)

add_executable(SW_BFC main.cpp
        util.cpp
        util.h
        Sample.cpp
        Sample.h
        hashfunction.cpp
        hashfunction.h
        SampleTable.cpp
        SampleTable.h
        EdgeTable.cpp
        EdgeTable.h
        NodeTable.cpp
        NodeTable.h
        Node.cpp
        Node.h
        Edge.h
        CandidateEdge.h
        BiGraph.cpp
        BiGraph.h
        exact_alg.cpp
        exact_alg.h
        Ams.cpp
        Ams.h
        def.h
)
