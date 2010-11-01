Include Complete
----------------

(c) John Mair (banisterfiend) 2010
MIT license

_Removes the shackles from Module#include_

Use `Module#include_complete` to bring in singleton classes from
modules. No more ugly ClassMethods and included() hook hacks.

* Install the [gem](https://rubygems.org/gems/include_complete): `gem install include_complete`
* Read the [documentation](http://rdoc.info/github/banister/include_complete/master/file/README.markdown)
* See the [source code](http://github.com/banister/include_complete)

example: include_complete():
----------------------------

Using `include_complete` the class methods of the
module will be mixed in along with the instance methods:

    
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
        include_complete M
    end

    # invoke class method
    A.hello #=> hello!

    # invoke instance method
    A.new.bye #=> bye!

Motivation
-----------

When a class inherits from another class it inherits both the
instance methods and class methods from its superclass.

Module inclusion does not work this way, only the module's instance methods
are mixed into the receiver's ancestor chain. This shortcoming
necessitates the `ClassMethods` included-hook-hack.

In my opinion this behaviour of modules violates the principle of
least surprise, though I'm aware not everyone agrees with this.

`include_complete` was written to make module inclusion work more like
class inheritance.

The extend_complete method
--------------------------

For completeness the `extend_complete` method has also been
implemented. Like traditional `extend` it mixes the module's instance
methods into the singleton class of the receiver. But where do the
singleton methods on the module end up? On the singleton class of the
singleton class of the receiver ;)

    module M
      def self.hello
        :hello
      end
    end
    
    class C
      extend_complete M
    
      class << self
        hello #=> :hello
      end
    end
        
As a result of this, it is unlikely `extend_complete` will be of much
use to anyone :)

How does it work?
-----------------

`include_complete` is a C extension that implements a highly modified
`rb_include_module()` function. Traditional module inclusion uses the
class pointer of the Included Module to point to the original module;
`include_complete` instead uses the class pointer to point to a
wrapped version of the singleton class of the module and stores the original module in a
hidden `__module__` instance variable. This wrapped singleton class is
then injected into the ancestor chain of the receiver's singleton
class.

Limitations
------------

`include_complete` uses a recursive function to generate the
Included Modules, and the base case of this recursion is reached when the singleton class of Module is
encountered. In the case where the module has a meta-meta class the recursive
function will not terminate and the program will hang.

It is highly unlikely and, as far as I know, next to useless for
a module to possess any higher order metaclasses so this limitation is
unlikely to be a problem in practice.

Criticisms
-----------

It may be argued that the current behaviour of modules is desirable
and that you do not in fact want module singleton classes to be mixed in
during inclusion. There are reasonably good arguments to
support this case which range from the obvious: That you do not want
your class to access hook methods on the module such as `included` and
`extended`. To the more subtle arguments: That a singleton
class is not a module and so *cannot* be mixed into an ancestor chain
dynamically.

Nonetheless, in my opinion, the advantages of `include_complete`
behaviour outweigh these considerations - It brings a nice symmetry
and consistency in behaviour to module inclusion and class
inheritance, it obviates the need for the included hook hack, and it
(in my opinion) correlates more closely with expectation and satisfies
the principle of least surprise.

Have a play, and decide for yourself :)

Companion Libraries
--------------------

include_complete is one of a series of experimental libraries that mess with
the internals of Ruby to bring new and interesting functionality to
the language, see also:

* [Remix](http://github.com/banister/remix) - Makes  ancestor chains fully read/write.
* [Object2module](http://github.com/banister/object2module) - Convert Classes and Objects to Modules so they can be extended/included
* [Prepend](http://github.com/banister/prepend) - Prepends modules in front of a class; so method lookup starts with the module
* [GenEval](http://github.com/banister/gen_eval) - A strange new breed of instance_eval


Contact
-------

Problems or questions contact me at [github](http://github.com/banister)

* Note: This project was previously called `real_include`

