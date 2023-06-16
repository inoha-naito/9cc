#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 3 'if (0) return 2; return 3;'
assert 3 'if (1-1) return 2; return 3;'
assert 2 'if (1) return 2; return 3;'
assert 2 'if (2-1) return 2; return 3;'
assert 4 'if (0) return 3; else return 4;'
assert 3 'if (1) return 3; else return 4;'

assert 55 'i=0; j=0; for (i=0; i<=10; i=i+1) j=i+j; return j;'

assert 10 'i=0; while (i<10) i=i+1; return i;'

echo OK
