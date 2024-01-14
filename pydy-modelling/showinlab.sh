#!/bin/sh
export PIPENV_VENV_IN_PROJECT=1
#rm Pipfile*
#pipenv --rm
.venv/bin/python -m jupyter lab ./rattleback-tensors.ipynb 
