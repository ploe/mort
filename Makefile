.PHONY: all clean lint venv src

all: src

src/cpu/%.c: src_%.py
		. ./venv/bin/activate; python $^

cpu: src/cpu/opcodes.c

src:
	. ./venv/bin/activate; python $^

venv: requirements.txt
	virtualenv -p python3 venv
	. ./venv/bin/activate; pip install -r requirements.txt

clean:
	rm -rv ./src
