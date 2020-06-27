"""Module that renders jinja2 templates from json"""

import argparse
import json

import jinja2


class Jinjify:
    """Opens json and renders jinja2 files"""

    @staticmethod
    def extract(filename):
        """Extract dict from filename"""
        with open(filename) as filehandle:
            return json.load(filehandle)

    @staticmethod
    def transform(extracted, filename):
        """Renders jinja2 template with json"""

        with open(filename) as filehandle:
            template = jinja2.Template(filehandle.read())

        return template.render(extracted=extracted)

    @staticmethod
    def load(transformed):
        """Print rendered template to stdout'"""
        print(transformed)

    def make(self, data, template):
        """Runs the make pipeline from start to finish"""
        extracted = self.extract(data)
        transformed = self.transform(extracted, template)
        return self.load(transformed)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('data', help='JSON data to render')
    parser.add_argument('template', help='Jinja2 template to render')
    args = vars(parser.parse_args())

    jinjify = Jinjify()
    jinjify.make(args['data'], args['template'])
