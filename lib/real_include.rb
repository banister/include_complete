# (c) 2010 John Mair (banisterfiend), MIT license

direc = File.dirname(__FILE__)

begin
  if RUBY_VERSION && RUBY_VERSION =~ /1.9/
    require "#{direc}/1.9/real_include"
  else
    require "#{direc}/1.8/real_include"
  end
rescue LoadError => e
  require 'rbconfig'
  dlext = Config::CONFIG['DLEXT']
  require "#{direc}/real_include.#{dlext}"
end

require "#{direc}/real_include/version"


class Module

  # include multiple modules (and their singletons) into an
  # inheritance chain
  # @param [Module] mods Modules to real_include
  # @return Returns the receiver
  # @example
  #   module M
  #     def self.hello
  #       puts "hello"
  #     end
  #   end
  #   class C
  #     real_include M
  #   end
  #   C.hello #=> "hello"
  def real_include(*mods)
    mods.each do |mod|
      real_include_one mod
    end
    self
  end
end
