import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class KeyboardCatRecipe(ConanFile):
    name = "keyboard_cat"
    version = "1.0"

    settings = "os", "build_type", "arch"
    generators = "CMakeDeps"

    exports_sources = "CMakeLists.txt", "keyboard_cat/*", "resources/*"

    def requirements(self):
        self.requires("sdl/3.2.6")
        self.requires("giflib/5.2.1")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = False
        tc.variables["SDL_SHARED"] = True
        tc.variables["SDLIMAGE_VENDORED"] = True
        tc.variables["SDLIMAGE_AVIF"] = False
        tc.variables["SDLIMAGE_BMP"] = False
        tc.variables["SDLIMAGE_JPEG"] = False
        tc.variables["SDLIMAGE_WEBP"] = False
        tc.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

        copy(self, "*.gif", src="resources", dst=os.path.join(self.package_folder, "resources"))
        copy(self, "*.bmp", src="resources", dst=os.path.join(self.package_folder, "resources"))

    def package_info(self):
        self.cpp_info.libs = ["keyboard_cat"]
