# -*- coding: utf-8 -*-
from distutils.core import setup, Extension

# Dynamically calculate the version based on src.VERSION.
version = __import__('version').get_version()

setup(name='stepik_alignment',
      version=version,
      ext_modules=[
            Extension('_stepik_alignment', sources=['_stepik_alignment_module.cc', 'stepik_alignment.cpp'])
      ],
      description='Stepik Sequence Alignment',
      )
