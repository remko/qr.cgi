import os, platform

vars = Variables("config.py")
vars.Add(BoolVariable("optimize", "Compile with optimizations turned on", "no"))
vars.Add(BoolVariable("debug", "Compile with debug information", "yes"))
vars.Add(BoolVariable("check", "Run unit tests", "no"))

env = Environment(ENV = {'PATH': os.environ['PATH']}, variables = vars)
Help(vars.GenerateHelpText(env))

# Compiler
if os.environ.get("CC", False) :
	env["CC"] = os.environ["CC"]
if os.environ.get("CXX", False) :
	env["CXX"] = os.environ["CXX"]

# Flags
if env["PLATFORM"] == "win32" :
	env.Append(LINKFLAGS = ["/INCREMENTAL:no"])
	env.Append(CCFLAGS = ["/EHsc", "/MD"])
	env.Append(CPPDEFINES = [("_WIN32_WINNT", "0x0501")])
	if env["debug"] :
		env.Append(CCFLAGS = ["/Zi"])
		env.Append(LINKFLAGS = ["/DEBUG"])
	# Workaround for broken SCons + MSVC2012 combo
	env["ENV"]["LIB"] = os.environ["LIB"]
	env["ENV"]["INCLUDE"] = os.environ["INCLUDE"]
else :
	env["CXXFLAGS"] = ["-std=c++11"]
	if env["debug"] :
		env.Append(CCFLAGS = ["-g"])
	if env["optimize"] :
		env.Append(CCFLAGS = ["-O2"])
	if env["PLATFORM"] == "posix" :
		env.Append(LIBS = ["pthread"])
		env.Append(CXXFLAGS = ["-Wextra", "-Wall"])
	elif env["PLATFORM"] == "darwin" :
		env["CC"] = "clang"
		env["CXX"] = "clang++"
		env["LINK"] = "clang++"
		if platform.machine() == "x86_64" :
			env["CCFLAGS"] = ["-arch", "x86_64"]
			env.Append(LINKFLAGS = ["-arch", "x86_64"])

# LibQREncode
libqrencode_flags = {
		"LIBS" : ["qrencode"]
}
if env["PLATFORM"] == "darwin" :
	libqrencode_flags.update({
		"CPPPATH": ["/usr/local/homebrew/include"],
		"LIBPATH": ["/usr/local/homebrew/lib"]
	})

# LibPNG
libpng_flags = {
		"LIBS": ["png"]
}

# Google test
gtest_flags = {
		"CPPPATH": ["3rdParty/gtest/include", "3rdParty/gtest"]
}
if env["PLATFORM"] == "win32" :
	# Workaround for missing variadic template support in VS2012
	gtest_flags["CPPDEFINES"] = [("_VARIADIC_MAX", 9)]


# Common library flags
env.MergeFlags(libqrencode_flags)
env.MergeFlags(libpng_flags)

# Core
core_env = env.Clone()
core = core_env.StaticLibrary("qrcgi", [
	"favicon.c",
	"HTTPResponse.cpp",
	"URL.cpp",
	"URLProvider.cpp",
	"CGIURLProvider.cpp",
	"QRCodeGenerator.cpp", 
	"QRCodeGeneratorImpl.cpp", 
	"QRCGI.cpp",
])

# Executable
prog_env = env.Clone()
prog_env.Program("qr.cgi", ["main.cpp"] + core)

# Tests
check_env = env.Clone()
check_env.MergeFlags(gtest_flags)
check_env.Replace(CXXFLAGS = [f for f in env["CXXFLAGS"] if not f.startswith("-W")])
unittests = check_env.Program("unittests", [
	"unittests.cpp",
	"URLTest.cpp",
	"CGIURLProviderTest.cpp",
	"QRCGITest.cpp",
	"HTTPResponseTest.cpp",
	"NullQRCodeGenerator.cpp",
] + core)
if env["check"] :
	check_env["ENV"]["GTEST_COLOR"] = 1
	check_env.Command("**dummy**", unittests, unittests[0].abspath)

# Favicon
def embed_file(env, target, source) :
	f = open(source[0].abspath, "r")
	data = f.read()
	f.close()
	data_bytes = bytearray(data)
	f = open(target[0].abspath, "w")
	f.write('#include <stddef.h>\n')
	f.write('const size_t ' + source[0].name.replace(".", "_") + "_size = " + str(len(data_bytes)) + ";\n")
	f.write('const unsigned char ' + source[0].name.replace(".", "_") + "[] = {" + ", ".join([str(b) for b in data_bytes]) + "};\n")
	f.close()
env.Command("favicon.c", ["favicon.ico"], embed_file)
env.Command("favicon.ico", "favicon.png", 
		"convert $SOURCE -bordercolor white -border 0 \\( -clone 0 -resize 16x16 \\) \\( -clone 0 -resize 32x32 \\) -delete 0 -colors 256 -transparent white $TARGET")

