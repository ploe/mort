"""Module that generates 'src/cpu/opcodes.c' files"""


import json
import os

import jinja2


class Maker:
    """Opens 'opcodes.json' and generates 'src/cpu/opcodes.c' files"""

    @staticmethod
    def extract():
        """Extract dict from opcodes.json"""
        with open('opcodes.json') as filehandle:
            return json.load(filehandle)

    @staticmethod
    def get_jumped_ignore(opcode):
        """Get the Jumped/Ignore values for cycles"""
        # Two cycles and it's a conditional jump
        if len(opcode['cycles']) > 1:
            return opcode['cycles'][0], opcode['cycles'][1]

        # One of these and it's always a jump
        if opcode['mnemonic'] in ('CALL', 'JP', 'JR', 'RET'):
            return opcode['cycles'][0], 0

        # Otherwise it's BAU
        return 0, opcode['cycles'][0]

    @staticmethod
    def transform(extracted):
        """Transform dicts in to correct format for template"""
        transformed = []
        for opcode in extracted['unprefixed'].values():
            if 'ILLEGAL' in opcode['mnemonic']:
                continue

            tag = opcode['mnemonic'].lower()
            for operand in opcode['operands']:
                tag = "{tag}_{operand}".format(
                    tag=tag, operand=operand['name'])

            opcode['tag'] = tag
            opcode['jumped'], opcode['ignore'] = Maker.get_jumped_ignore(
                opcode)

            transformed.append(opcode)

        with open('jinja2/templates/src/opcodes.j2') as filehandle:
            template = jinja2.Template(filehandle.read())
            return template.render(opcodes=transformed)

    @staticmethod
    def load(transformed):
        """Loaded transformed dicts in to 'src/cpu/opcodes.c'"""

        os.makedirs('build/src/cpu', exist_ok=True)

        with open('build/src/cpu/opcodes.c', 'w') as filehandle:
            filehandle.write(transformed)

    def make(self):
        """Runs the make pipeline from start to finish"""
        extracted = self.extract()
        transformed = self.transform(extracted)
        return self.load(transformed)


if __name__ == '__main__':
    maker = Maker()
    maker.make()
