require './real_include'

module M
    def self.hello
        puts "hello"
    end
end

class A
    real_include M
end
