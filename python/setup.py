# setup.py
from setuptools import setup, find_packages

setup(
    name="spip",
    version="2.0",
    packages=find_packages(),  # Automatically find your "mymodule"
    install_requires=["astropy>=5.3",
                      "uics>=1.0"]
)
