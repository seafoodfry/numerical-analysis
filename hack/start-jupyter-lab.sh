#!/bin/bash
set -e
set -x

source $(poetry env info --path)/bin/activate

jupyter lab --ip=0.0.0.0