#!/usr/bin/env sh

bin="./bin/inet-lisp benchmark"
ext=lisp

for example in $(find examples -name "*.benchmark.${ext}"); do
    ${bin} $example
done
