# printer

`frame_print` -- pure lispy printer
`worker_print` -- pure lispy printer

[bug] `DEBUG_STEP_LOG 1` can not pass tests -- data race

```
./bin/inet-lisp run -p examples/readme/nat.test.lisp
```

[bug] `--single-threaded` flag print extra top level

```
./bin/inet-lisp run -s -p examples/primitives/top-level-exp.test.lisp
```

# bug

primitive should not have `input_arity` and `output_arity` -- should only have `arity`
`apply_n` as primitive node ctor
support `(apply)`

[bug] apply unfuzed wire

```
./bin/inet-lisp run -p -s examples/datatypes/list-map.play.lisp
```

# example

[example] more examples from the paper

# deque

[deque] `deque_t` -- use mod queue for real -- not just list + lock

# core

[core] `node_t` -- use `spinlock_t`

# lang

[lang] `print_connected` -- be more lispy
[lang] refactor `define_rule_star`

# player

[player] bring back player
[player] use real physics force
[player] `node_physics_simulate` -- move by `velocity` and clear `force` for every `node_model`
[player] remove `node_physics_fake_simulate`
[player] `draw_net` -- print non-wire value -- test by `list-map.play.lisp`

# arity checker

[lang] [maybe] write arity checker in inet-lisp itself!
[lang] write the arity checker like a type checker -- choose good names in the problem domain
[lang] check arity -- compute arity from exp
[lang] compute `arity` from each exp -- what should be the function names?
[lang] `compile_exp` -- `EXP_AP` -- should not get `arity` from `list_length(exp->ap.arg_list)`
[lang] `apply_function` -- supplement wire if arity is short

# error report

improve undefined-node.error.lisp

# testing

[testing] `(assert)` report position
