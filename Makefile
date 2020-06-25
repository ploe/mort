.PHONY: all clean lint venv src cpu

all: src

build/src/cpu/%.c: jinja2/%.py
		. ./venv/bin/activate; python $^

build/src/cpu/%: jinja2/%.py
	. ./venv/bin/activate; python $^

cpu: build/src/cpu/opcodes.c
cpu: build/src/cpu/instructions

venv: requirements.txt
	virtualenv -p python3 venv
	. ./venv/bin/activate; pip install -r requirements.txt

clean:
	rm -rv ./build
