src: lint
	. ./venv/bin/activate; python src_cpu.py

lint: src_cpu.py
	. ./venv/bin/activate; \
	autopep8 --in-place --aggressive --aggressive $<; \
	pylint $<;

clean:
	rm -rv ./src
