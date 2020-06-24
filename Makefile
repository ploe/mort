.PHONY: all clean lint

all: src

src: lint
	. ./venv/bin/activate; python src_cpu.py

lint: src_cpu.py venv
	. ./venv/bin/activate; \
	autopep8 --in-place --aggressive --aggressive $<; \
	pylint $<;

venv: requirements.txt
	virtualenv -p python3 venv
	. ./venv/bin/activate; pip install -r requirements.txt

clean:
	rm -rv ./src
