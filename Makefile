.PHONY: all clean src cpu

all: build cpu
	cp -rv include/* build/include
	cc -o mort main.c build/src/cpu/opcodes.c build/src/cpu/instructions/*.c -Ibuild/include

build:
	mkdir -p build/src/cpu build/include

build/src/cpu/%.c: jinja2/templates/src/%.j2
		. ./venv/bin/activate; python jinja2/jinjify.py jinja2/opcodes.json $< > $@

#cpu: venv
cpu: build/src/cpu/opcodes.c
#cpu: build/src/cpu/instructions

venv: requirements.txt
	virtualenv -p python3 venv
	. ./venv/bin/activate; pip install -r requirements.txt

clean:
	rm -rv ./build
