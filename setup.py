from distutils.core import setup, Extension

alaya_module = Extension('alaya',
                    define_macros = [],
                    include_dirs = ['/usr/local/include'],
                    libraries = [],
                    library_dirs = [],
                    sources = ['src/alaya.C', 'src/pyalaya.C', 'src/log.C', 'src/mongoose.c'])

setup (name = 'PackageName',
       version = '1.0',
       description = 'This is a demo package',
       author = 'Martin v. Loewis',
       author_email = 'martin@v.loewis.de',
       url = 'http://docs.python.org/extending/building',
       long_description = '''
This is really just a demo package.
''',
       ext_modules = [module1])

