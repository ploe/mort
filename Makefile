.PHONY: all clean src cpu

all: build cpu
	cp -rv include/* build/include
#	cc -o mort main.c build/src/*.c -Ibuild/include

build:
	mkdir -p build/src build/include

build/src/%.c: jinja2/templates/src/%.j2
		. ./venv/bin/activate; python jinja2/jinjify.py json jinja2/opcodes.json $< > $@

cpu: venv
cpu: build/src/opcodes.c

venv: requirements.txt
	virtualenv -p python3 venv
	. ./venv/bin/activate; pip install -r requirements.txt

clean:
	rm -rv ./build
