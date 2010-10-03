require './real_include'

module N
  def self.hello
    puts "hello!"
  end
end

module M
  real_include N
end

