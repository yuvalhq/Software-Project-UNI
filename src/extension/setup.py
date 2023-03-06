import os
from glob import glob

from setuptools import Extension, setup

sources = [
    os.path.abspath(f)
    for f in glob(os.path.join(os.path.dirname(__file__), "*.c"))
    + glob(os.path.join(os.path.dirname(__file__), "..", "spkmeans/*.c"))
]

includes = [
    os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "spkmeans")),
    os.path.abspath(os.path.dirname(__file__)),
]

module = Extension(
    "spkmeans_c",
    sources=sources,
    include_dirs=includes,
)

setup(
    name="spkmeans_c",
    version="1.0",
    description="spkmeans C implementation",
    ext_modules=[module],
)
