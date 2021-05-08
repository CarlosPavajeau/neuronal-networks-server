find_package (Eigen3 REQUIRED NO_MODULE)
add_library(Eigen3 INTERFACE)

target_link_libraries(Eigen3
        INTERFACE
        Eigen3::Eigen)
