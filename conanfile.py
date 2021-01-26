from conans import ConanFile, AutoToolsBuildEnvironment, MSBuild, tools, CMake
import os


class resize_images(ConanFile):
	name = "resize_images"
	version = "0.1"
	settings = "os", "compiler", "build_type", "arch"
	generators = "cmake", "gcc", "visual_studio"
	#default_options = {"*:shared" = False}
	no_copy_source = True
	build_policy = "missing"
	
	def build_requirements(self):
		self.build_requires("cmake/3.19.3")
		if self.settings.os == "Windows":
			self.build_requires("mingw_installer/1.0@conan/stable")
			self.build_requires("msys2/20200517")
		
	def requirements(self):
		# installed by boost library 
		self.requires("zlib/1.2.11")
		self.requires("libpng/1.6.37")
		self.requires("libjpeg/9d")
		self.requires("boost/1.71.0@conan/stable")
		
	def install(self):		
		self.options["boost"].without_atomic = True
		self.options["boost"].without_chrono = True
		self.options["boost"].without_container = True
		self.options["boost"].without_context = True
		self.options["boost"].without_contract = True
		self.options["boost"].without_coroutine = True
		self.options["boost"].without_date_time = True
		self.options["boost"].without_exception = True
		self.options["boost"].without_fiber = True
		self.options["boost"].without_filesystem = False
		self.options["boost"].without_graph = True
		self.options["boost"].without_graph_parallel = True
		self.options["boost"].without_iostreams = True
		self.options["boost"].without_locale = True
		self.options["boost"].without_log = True
		self.options["boost"].without_math = True
		self.options["boost"].without_mpi = True
		self.options["boost"].without_program_options = False
		self.options["boost"].without_python = True
		self.options["boost"].without_random = True
		self.options["boost"].without_regex = False
		self.options["boost"].without_serialization = True
		self.options["boost"].without_stacktrace = True
		self.options["boost"].without_system = False 
		self.options["boost"].without_test = True
		self.options["boost"].without_thread = True
		self.options["boost"].without_timer = True
		self.options["boost"].without_type_erasure = True
		self.options["boost"].without_wave = True

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


	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()
