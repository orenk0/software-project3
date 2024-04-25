from setuptools import Extension, setup

module = Extension("mysymnmf",
                  sources=[
                    'symnmf.c','symnmfmodule.c','symnmf.h'
                  ])
setup(name='mysymnmf',
     version='1.0',
     description='calculating the matrixs from 1',
     ext_modules=[module])