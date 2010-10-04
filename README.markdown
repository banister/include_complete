Real Include 
--------------

(c) John Mair (banisterfiend) 
MIT license

Removes the shackles from Module#include - use Module#real_include to
bring in singleton classes from modules. No more ugly ClassMethods and
included() hook hacks.

** This is BETA software and has not yet been thoroughly tested, use
   at own risk **

install the gem: **for testing purposes only**
`gem install real_include`

example: 

    module M
        # class method
        def self.hello
            puts "hello!"
        end

        # instance method
        def bye
            puts "bye!"
        end
    end

    class A
        real_include M
    end

    # invoke class method
    A.hello #=> hello!

    # invoke instance method
    A.new.bye #=> bye!

