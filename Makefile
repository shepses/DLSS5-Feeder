
.PHONY: build
build:	
	# Get git commit hash for versioning
	FEED_BUILD_ID=$(git rev-parse --short HEAD 2>/dev/null || echo "unknown")

	# Create build directory
	mkdir -p build

	echo "[*] Compiling MinMinHook and DLSS5-Feed for Windows using MinGW-Clang..."

	x86_64-w64-mingw32-windres -Iexternal/reshade/include -Iexternal/ngx \
    	src/version.rc -O coff -o build/version.o

	x86_64-w64-mingw32-clang++ -shared -std=c++20 -O2 -Wall -fms-extensions \
			-DFEED_BUILD_ID="\"$(git rev-parse --short HEAD 2>/dev/null || echo "unknown")\"" \
			-Iexternal/reshade/include \
			-Iexternal/ngx/include \
			-Iexternal/vulkan/include \
			-Iexternal/imgui \
			-Iexternal/minhook/include \
			src/dlss5-feed.cpp \
			src/compat_linux.c \
			external/minhook/src/buffer.c \
			external/minhook/src/hook.c \
			external/minhook/src/trampoline.c \
			external/minhook/src/hde/hde64.c \
			build/version.o \
			external/ngx/lib/nvsdk_ngx_d.lib \
			-lucrt -lvcruntime -lmsvcrt \
			-lversion -lkernel32 -luser32 -ladvapi32 -lole32 \
			-o build/dlss5-feed.addon64 \
			-Wl,--out-implib,build/dlss5-feed.lib \
			-Wl,--allow-multiple-definition

	echo "[+] Build complete: build/dlss5-feed.addon64"