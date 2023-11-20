#!/bin/bash

temp_dir=$(mktemp -d)

cp -r --parents "hashtable/hashtable.c" "$temp_dir"
cp -r --parents "btree/rec/btree.c" "$temp_dir"
cp -r --parents "btree/iter/btree.c" "$temp_dir"
cp -r --parents "btree/exa/exa.c" "$temp_dir"

tar -cf "xcuprm01.tar" -C "$temp_dir" hashtable btree

rm -r "$temp_dir"
# author ~ Sir Bedevere #
