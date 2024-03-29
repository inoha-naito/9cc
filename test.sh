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
assert 3 'for (;;) return 3; return 5;'

assert 10 'i=0; while (i<10) i=i+1; return i;'

assert 10 "a = 0;
for(;;a = a + 1) if (a == 5) return 10;
return 2;"

# block
assert 10 "a = 0;
for(;;) {
  a = a + 1;
  if (a == 5) return 10;
}
return 2;"

echo OK
