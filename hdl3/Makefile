all: bin/sample bin/hc
	./bin/hc test/sample.h3
	@echo done

no:
	wat2wasm wasi-poll-stdin.wat
	(wasmtime wasi-poll-stdin.wasm ; echo $?)	
	./bin/zc
	wat2wasm test.wat
	zcat src/3o3_exe.cmd.gz > test.cmd
	cat test.wasm >> test.cmd
	node test.js


www: all
	http-server

bin/hc: src/hc.h3
	cat src/hc.h3 | ./bin/h3 > bin/hc.c
	gcc -g -Wall -ansi bin/hc.c -o bin/hc


bin/sample: test/sample.h3 bin/h3
	cat test/sample.h3 | ./bin/h3 > bin/s.c
	gcc -g -Wall -ansi bin/s.c -o bin/sample

bin/h3: src/h3.c
	mkdir -p bin
	cc -g -Wall -o bin/h3 src/h3.c

clean:
	rm -f bin/*

fmt:
	tidy -i -w 1024 --preserve-entities yes --show-info no --warn-proprietary-attributes no -m *.html
	js-beautify -n --no-preserve-newlines -w 79 -s 2 *.js
	clang-format --style="{BasedOnStyle: llvm,UseTab: Always,IndentWidth: 8,TabWidth: 8}" -i src/*.h src/*.c

	
