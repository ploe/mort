.PHONY: all clean src cpu

all: cpu
	cp -rv include/* build/include
	cc -o mort main.c build/src/cpu/opcodes.c build/src/cpu/instructions/*.c -Ibuild/include

build/src/cpu/%.c: jinja2/%.py
		. ./venv/bin/activate; python $^

build/src/cpu/%: jinja2/%.py
	. ./venv/bin/activate; python $^

cpu: venv
cpu: build/src/cpu/opcodes.c
cpu: build/src/cpu/instructions

venv: requirements.txt
	virtualenv -p python3 venv
	. ./venv/bin/activate; pip install -r requirements.txt

clean:
	rm -rv ./build
