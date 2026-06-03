#!/bin/bash
set -e
echo "Cloning the DOSLIB and BIOSLIB submodules"
git clone --recurse-submodules https://github.com/cspjst/DOSLIB.git
