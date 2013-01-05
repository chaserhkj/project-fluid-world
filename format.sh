#!/bin/sh
path=$(dirname $0)
uncrustify -c $path/uncrustify.cfg --no-backup $@
astyle -A8s4YfpHUck2W2 $@

