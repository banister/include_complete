# (c) 2010 John Mair (banisterfiend), MIT license

direc = File.dirname(__FILE__)

begin
  if RUBY_VERSION && RUBY_VERSION =~ /1.9/
    require "#{direc}/1.9/include_complete"
  else
    require "#{direc}/1.8/include_complete"
  end
rescue LoadError => e
  require 'rbconfig'
  dlext = Config::CONFIG['DLEXT']
  require "#{direc}/include_complete.#{dlext}"
end

require "#{direc}/include_complete/version"


class Module

  # include modules (and their singletons) into an
  # inheritance chain
  # @param [Module] mods Modules to include_complete
  # @return Returns the receiver
  # @example
  #   module M
  #     def self.hello
  #       puts "hello"
  #     end
  #   end
  #   class C
  #     include_complete M
  #   end
  #   C.hello #=> "hello"
  def include_complete(*mods)
    mods.each do |mod|
      include_complete_one mod
    end
    self
  end
end

class Object

  # extend modules (and their singletons) into an
  # inheritance chain
  # @param [Module] mods Modules to extend_complete
  # @return Returns the receiver
  # @example
  #   module M
  #     def self.hello
  #       puts "hello"
  #     end
  #   end
  #   o = Object.new
  #   o.extend_complete M
  #   o.singleton_class.hello #=> "hello"
  def extend_complete(*mods)
    class << self; self; end.send(:include_complete, *mods)
  end
end
