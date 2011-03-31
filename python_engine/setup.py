#!/usr/bin/env python

import sys
sys.prefix = '/usr'
sys.exec_prefix = '/usr'

from distutils.core import setup
from distutils.extension import Extension

setup(name='scge',
      version='0.1',
      description='Simple C++ Game Engine',
      author='JacobF',
      author_email='queatz@gmail.com',
      url='http://www.queatz.com',
      packages=['scge'],
      package_data={'scge': ['_scge.so']},
)
