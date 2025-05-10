#!/bin/bash
set -e
set -x

uv run jupyter lab --ip=0.0.0.0
