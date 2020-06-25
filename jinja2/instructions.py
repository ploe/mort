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
        for key, opcode in extracted['unprefixed'].items():
            if 'ILLEGAL' in opcode['mnemonic']:
                continue

            filename = "build/src/cpu/instructions/{mnemonic}.c".format(
                **opcode).lower()

            opcode['index'] = int(key, 0)
            opcode['key'] = key

            tag = opcode['mnemonic'].lower()
            for operand in opcode['operands']:
                tag = "{tag}_{operand}".format(
                    tag=tag, operand=operand['name'])

            opcode['tag'] = tag

            if len(opcode['cycles']) > 1:
                opcode['jump'] = True

            src = transformed.get(filename, [])
            src.append(opcode)
            transformed[filename] = src

        for filename in transformed:
            opcodes = sorted(
                transformed[filename], key=lambda k: k['tag'])

            with open('jinja2/templates/src/instructions.j2') as filehandle:
                template = jinja2.Template(filehandle.read())

            code = template.render(filename=filename, opcodes=opcodes)

            transformed[filename] = code

        return transformed

    @staticmethod
    def load(transformed):
        """Loaded transformed dicts in to 'build/src/cpu/instructions/{mnemonic}.c'"""

        os.makedirs('build/src/cpu/instructions', exist_ok=True)

        for filename, code in transformed.items():
            with open(filename, 'w') as filehandle:
                filehandle.write(code)

    def make(self):
        """Runs the make pipeline from start to finish"""
        extracted = self.extract()
        transformed = self.transform(extracted)
        return self.load(transformed)


if __name__ == '__main__':
    maker = Maker()
    maker.make()
