#!/bin/sh
export PIPENV_VENV_IN_PROJECT=1
rm Pipfile*
pipenv --rm
pipenv install
pipenv run pip install -r requirements.txt 
.venv/bin/python -m jupyter lab build
