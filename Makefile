.PHONY: build clean run

build_type := Debug
target := fenix_editor

generate:
	# ----------------------
	# Generating build files
	# ----------------------
	@mkdir -p build
	@cmake -G 'Unix Makefiles' -S . -B build -DCMAKE_BUILD_TYPE=$(build_type) \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=YES -DWARNINGS_AS_ERRORS=FALSE

build: generate
	# ---------
	# Compiling
	# ---------
	@cmake --build build -j 4

run: build
	# -------------------
	# Running application
	# -------------------
	@./build/bin/$(build_type)/$(target)

clean:
	rm -rf build *.log

rebuild: clean build
