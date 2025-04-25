#!/usr/bin/env bash

bin="./bin/inet-lisp run --print-top-level-exp"
ext=lisp

for example in $(find examples -name "*.${ext}" -not -name "*.test.${ext}" -not -name "*.error.${ext}" -not -name "*.play.${ext}"); do
    echo "[run] $example"
    ${bin} $example
done

for example in $(find examples -name "*.test.${ext}"); do
    echo "[test] $example"
    ${bin} $example > $example.out
done

for example in $(find examples -name "*.error.${ext}"); do
    echo "[error] $example"
    ${bin} $example > $example.err || true
done
