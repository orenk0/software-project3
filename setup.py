from setuptools import Extension, setup

## might have to add ,'symnmf.h'in source(delete before submition)
module = Extension("mysymnmf",
                  sources=[
                    'symnmf.c','symnmfmodule.c'
                  ])
setup(name='mysymnmf',
     version='1.0',
     description='calculating the matrixs from 1',
     ext_modules=[module])
