[lang] `print_connected` -- be more lispy
[lang] refactor `define_rule_star`

# primitive

[primitive] bring back player support
[core] fix `worker_apply` for function

# example

[example] more examples from the paper

# deque

[deque] `deque_t` -- use mod queue for real -- not just list + lock

# core

[core] `node_t` -- use `spinlock_t`

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
