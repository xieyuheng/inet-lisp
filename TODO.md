# thread

[thread] `thread_pool_t` -- one level of indirect
[thread] `thread_pool_start`
[thread] `thread_pool_join`

# counter

[counter] `stats_counter_t` -- to provide statistics to human, fast per-thread write, slow summary read.
[counter] `limit_counter_t`
[counter] `zero_counter_t`

# deque

[deque] `deque_t` -- use mod queue for real -- not just list + lock

# tak

[core] `(if)` as special syntax

[prelude] int-lt?
[prelude] int-gt?
[prelude] int-lteq?
[prelude] int-gteq?

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

# player

[player] bring back player
[player] use real physics force
[player] `node_physics_simulate` -- move by `velocity` and clear `force` for every `node_model`
[player] remove `node_physics_fake_simulate`
[player] `draw_net` -- print non-wire value -- test by `list-map.play.lisp`
