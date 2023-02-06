from glob import glob
from setuptools import Extension, setup

module = Extension("spkmeans_c", sources=glob("*.c"))
setup(
    name="spkmeans_c",
    version="1.0",
    description="spKmeans C implementation",
    ext_modules=[module],
)
