#!/bin/sh
# run inside dosfxp or retrolib to swap embedded doslib/ for a real submodule
rm -rf doslib
git submodule add https://github.com/cspjst/DOSLIB.git doslib
git submodule update --init --recursive
git add .gitmodules doslib
git commit -m "Add doslib as a proper submodule"
