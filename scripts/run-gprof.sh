gprof ./bin/inet-lisp tmp/gmon.out | gprof2dot | dot -Gdpi=200 -Tpng -o tmp/out.png
