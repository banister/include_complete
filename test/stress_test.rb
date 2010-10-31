require '../lib/real_include'

5000.times {
  m = Module.new
  n = Module.new
  k = Module.new
  n.real_include m
  k.real_include n

  c = Class.new
  c.real_include k
}

"stress test passed!".display
