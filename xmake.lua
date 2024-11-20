add_rules("mode.debug", "mode.release")

add_requires("glfw")
add_requires('imgui', {configs = {glfw_opengl3 = true}})
add_requires("glm")
add_requires("assimp")

set_languages("cxx20")

target("glad")
    set_kind("static")
    add_includedirs("lib/glad/include", {public = true})
    add_files("lib/glad/src/glad.c")

target("game")
    set_kind("binary")
    add_files("src/**.cpp")
    add_deps("glad")
    add_packages("glfw", "imgui", "glm", "assimp")
    
    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)/")
    add_configfiles("shaders/*", {onlycopy = true, prefixdir = "shaders/"})
    add_configfiles("models/*", {onlycopy = true, prefixdir = "models/"})
