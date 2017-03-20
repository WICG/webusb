#!/bin/bash
set -e # Exit with nonzero exit code if anything fails.

for SPEC in $SPECS; do
  echo ${SPEC}.bs:
  ERR=$(curl https://api.csswg.org/bikeshed/ -f -F file=@${SPEC}.bs -F output=err)
  if [ -n "$ERR" ]; then
    echo "$ERR"
    exit 1
  fi
done
