#
# CGRA Project
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"cgra_geometry.hpp"
	"cgra_math.hpp"
	"opengl.hpp"
	"simple_shader.hpp"
	"simple_image.hpp"
	"terrain.hpp"
	"noise.hpp"
	"flocking.hpp"
	"geometry.hpp"

)


# TODO list your source files (.cpp) here
SET(sources
	"main.cpp"
	"noise.cpp"
	"geometry.cpp"
	"terrain.cpp"
	"flocking.cpp"
)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${CGRA_PROJECT} ${headers} ${sources})
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE stb)
