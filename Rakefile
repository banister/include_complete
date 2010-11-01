$LOAD_PATH.unshift File.join(File.expand_path(__FILE__), '..')

direc = File.dirname(__FILE__)
dlext = Config::CONFIG['DLEXT']

require 'rake/clean'
require 'rake/gempackagetask'
require './lib/include_complete/version'

CLEAN.include("ext/**/*.#{dlext}", "ext/**/*.log", "ext/**/*.o", "ext/**/*~", "ext/**/*#*", "ext/**/*.obj", "ext/**/*.def", "ext/**/*.pdb")
CLOBBER.include("**/*.#{dlext}", "**/*~", "**/*#*", "**/*.log", "**/*.o")

def apply_spec_defaults(s)
  s.name = "include_complete"
  s.summary = "Fixing the limitations in traditional Module#include"
  s.version = IncludeComplete::VERSION
  s.date = Time.now.strftime '%Y-%m-%d'
  s.author = "John Mair (banisterfiend)"
  s.email = 'jrmair@gmail.com'
  s.description = s.summary
  s.require_path = 'lib'
  s.homepage = "http://banisterfiend.wordpress.com"
  s.has_rdoc = 'yard'
  s.files =  FileList["Rakefile", "README.markdown", "CHANGELOG", 
                      "lib/**/*.rb", "ext/**/extconf.rb", "ext/**/*.h",
                      "ext/**/*.c", "test/**/*.rb"].to_a 
end

task :test do
  sh "bacon -k #{direc}/test/test.rb"
end

[:mingw32, :mswin32].each do |v|
  namespace v do
    spec = Gem::Specification.new do |s|
      apply_spec_defaults(s)        
      s.platform = "i386-#{v}"
      s.files += FileList["lib/**/*.#{dlext}"].to_a
    end

    Rake::GemPackageTask.new(spec) do |pkg|
      pkg.need_zip = false
      pkg.need_tar = false
    end
  end
end

namespace :ruby do
  spec = Gem::Specification.new do |s|
    apply_spec_defaults(s)        
    s.platform = Gem::Platform::RUBY
    s.extensions = ["ext/include_complete/extconf.rb"]
  end

  Rake::GemPackageTask.new(spec) do |pkg|
    pkg.need_zip = false
    pkg.need_tar = false
  end
end
