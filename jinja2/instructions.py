"""Module that generates 'build/src/cpu/instructions/{mnemonic}.c' files"""


import json
import os

import jinja2


class Maker:
    """Opens 'opcodes.json' and generates 'build/src/cpu/instructions/{mnemonic}.c' files"""

    @staticmethod
    def extract():
        """Extract dict from opcodes.json"""
        with open('opcodes.json') as filehandle:
            return json.load(filehandle)

    @staticmethod
    def transform(extracted):
        """Transform dicts in to correct format for template"""
        transformed = {}
        prototypes = []
        for opcode in extracted['unprefixed'].values():
            if 'ILLEGAL' in opcode['mnemonic']:
                continue

            filename = "build/src/cpu/instructions/{mnemonic}.c".format(
                **opcode).lower()

            tag = opcode['mnemonic'].lower()
            for operand in opcode['operands']:
                keys = {'increment': 'inc', 'decrement': 'dec'}
                for operator, value in keys.items():
                    if operand.get(operator, False):
                        tag = "{tag}_{value}".format(tag=tag, value=value)


                prefix = "a"
                if operand['immediate']:
                    prefix = "r"

                tag = "{tag}_{prefix}{operand}".format(
                    tag=tag, prefix=prefix, operand=operand['name'])

            opcode['tag'] = tag

            if len(opcode['cycles']) > 1:
                opcode['jump'] = True

            src = transformed.get(filename, [])
            src.append(opcode)
            transformed[filename] = src

            prototypes.append(opcode)

        files = {
            'src': {},
        }

        for filename in transformed:
            opcodes = sorted(
                transformed[filename], key=lambda k: k['tag'])

            with open('jinja2/templates/src/instructions.j2') as filehandle:
                template = jinja2.Template(filehandle.read())

            files['src'][filename] = template.render(
                filename=filename, opcodes=opcodes)

        with open('jinja2/templates/include/instructions.j2') as filehandle:
            template = jinja2.Template(filehandle.read())

        files['include'] = template.render(opcodes=prototypes)

        return files

    @staticmethod
    def load(transformed):
        """Loaded transformed dicts in to 'build/src/cpu/instructions/{mnemonic}.c'"""

        os.makedirs('build/src/cpu/instructions', exist_ok=True)

        for filename, code in transformed['src'].items():
            with open(filename, 'w') as filehandle:
                filehandle.write(code)

        os.makedirs('build/include/mort', exist_ok=True)
        with open('build/include/mort/instructions.h', 'w') as filehandle:
            filehandle.write(transformed['include'])

    def make(self):
        """Runs the make pipeline from start to finish"""
        extracted = self.extract()
        transformed = self.transform(extracted)
        return self.load(transformed)


if __name__ == '__main__':
    maker = Maker()
    maker.make()
