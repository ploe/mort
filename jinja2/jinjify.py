"""Module that renders jinja2 templates from json"""

import argparse
import csv
import json

import jinja2


class Jinjify:
    """Opens json and renders jinja2 files"""

    def extract(self, doctype, filename):
        """Extract dict from filename"""
        with open(filename) as filehandle:
            tag = "{doctype}_extract".format(doctype=doctype)
            method = getattr(self, tag, self.not_implemented)

            if not callable(method):
                method = self.not_implemented

            extracted = method(filehandle)

        return extracted

    @staticmethod
    def not_implemented(filehandle):
        """Raise TypeError if extract function does not exist"""
        raise TypeError

    @staticmethod
    def json_extract(filehandle):
        """Use JSON to extract the data"""
        return json.load(filehandle)

    @staticmethod
    def csv_extract(filehandle):
        """Use CSV to extract the data"""
        reader = csv.DictReader(filehandle)
        extracted = []
        for row in reader:
            extracted.append(row)

        return extracted

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

    def make(self, doctype, data, template):
        """Runs the make pipeline from start to finish"""
        extracted = self.extract(doctype, data)
        transformed = self.transform(extracted, template)
        return self.load(transformed)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('doctype', help='The format of the data')
    parser.add_argument('data', help='JSON data to render')
    parser.add_argument('template', help='Jinja2 template to render')
    args = vars(parser.parse_args())

    jinjify = Jinjify()
    jinjify.make(args['doctype'], args['data'], args['template'])
