import os
from glob import glob

from setuptools import Extension, setup

SRC_CODE_DIR_NAME = "spkmeans_c"

sources = [
    os.path.abspath(f)
    for f in glob(os.path.join(os.path.dirname(__file__), "*.c"))
    + glob(os.path.join(os.path.dirname(__file__), "..", f"{SRC_CODE_DIR_NAME}/*.c"))
]

includes = [
    os.path.abspath(os.path.join(os.path.dirname(__file__), "..", SRC_CODE_DIR_NAME)),
    os.path.abspath(os.path.dirname(__file__)),
]

module = Extension(
    "mykmeanssp",
    sources=sources,
    include_dirs=includes,
)

setup(
    name="mykmeanssp",
    version="1.0",
    description="spkmeans C implementation",
    ext_modules=[module],
)
