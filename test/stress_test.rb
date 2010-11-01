direc = File.dirname(__FILE__)
require "#{direc}/../lib/include_complete"

5000.times {
  m = Module.new
  n = Module.new
  k = Module.new
  n.include_complete m
  k.include_complete n

  c = Class.new
  c.include_complete k
}

"stress test passed!".display
