require './real_include'

module N
  def self.hello
    puts "hello!"
  end
end

module M
  real_include N

  def self.blah
  end
end

M.hello

puts M.ancestors.inspect

class A
real_include M
end

puts A.ancestors.inspect
