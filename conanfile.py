from conans import ConanFile, AutoToolsBuildEnvironment, MSBuild, tools, CMake
import os


class resize_images(ConanFile):
	name = "resize_images"
	version = "0.1b"
	settings = "os", "compiler", "build_type", "arch"
	generators = "cmake", "xcode"
	no_copy_source = True
	build_policy = "missing"
	options = {"shared": [True, False], "st": [True, False]}
	default_options = {"shared": False, "st": False}
            
	def requirements(self):
		# installed by boost library 
		self.requires("zlib/1.2.11")
		self.requires("libpng/1.6.37")
		self.requires("libjpeg/9d")

	def package(self):
		self.copy("*.h")
		self.copy("*.hpp")

	def _run_cmd(self, command):
		if self.settings.os == "Windows":
			tools.run_in_windows_bash(self, tools.unix_path(command))
		else:
			self.run(command)

	def _build_autotools(self, configure_args=None):
		env_build = AutoToolsBuildEnvironment(self, win_bash=tools.os_info.is_windows)
		env_build.configure(args=configure_args)
		env_build.make()
		env_build.make(args=["install"])

	def configure_cmake(self):
		cmake = CMake(self)

		if self.settings.os == "Windows":
			cmake.definitions["CONAN_SHARED_LINKER_FLAGS"] = "-static -static-libstdc++"
		
		cmake.configure()
		return cmake

	def build(self):
		cmake = self.configure_cmake()
		cmake.build()
		# run unit tests after the build
        #cmake.test()

        # run custom make command
        #self.run("make -j3 check)
