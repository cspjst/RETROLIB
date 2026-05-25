#!/bin/bash
set -e
echo "Updating BIOSLIB inside DOSLIB..."
git -C doslib submodule update --remote --merge
git -C doslib add bioslib
git -C doslib commit -m "Sync BIOSLIB to latest" || echo "BIOSLIB already up to date"

echo "Updating DOSLIB..."
git -C doslib add -A
git -C doslib commit -m "Sync submodules" || echo "DOSLIB already up to date"

echo "Staging DOSLIB in RETROLIB..."
git add doslib
git commit -m "Sync DOSLIB to latest" || echo "RETROLIB already up to date"

echo "Done."
