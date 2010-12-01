$LOAD_PATH.unshift File.join(File.expand_path(__FILE__), '..')

direc = File.dirname(__FILE__)
dlext = Config::CONFIG['DLEXT']

require 'rake/clean'
require 'rake/gempackagetask'
require './lib/include_complete/version'

CLOBBER.include("**/*.#{dlext}", "**/*~", "**/*#*", "**/*.log", "**/*.o")
CLEAN.include("ext/**/*.#{dlext}", "ext/**/*.log", "ext/**/*.o", "ext/**/*~",
              "ext/**/*#*", "ext/**/*.obj", "ext/**/*.def",
              "ext/**/*.pdb", "*flymake", "*flymake.*")

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
  s.files =  Dir["Rakefile", "README.markdown", "CHANGELOG", 
                      "lib/**/*.rb", "ext/**/extconf.rb", "ext/**/*.h",
                      "ext/**/*.c", "test/**/*.rb"]
end

task :test do
  sh "bacon -k #{direc}/test/test.rb"
end

[:mingw32, :mswin32].each do |v|
  namespace v do
    spec = Gem::Specification.new do |s|
      apply_spec_defaults(s)        
      s.platform = "i386-#{v}"
      s.files += Dir["lib/**/*.#{dlext}"]
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

directories = ["#{direc}/lib/1.8", "#{direc}/lib/1.9"]
directories.each { |d| directory d }

desc "build the 1.8 and 1.9 binaries from source and copy to lib/"
task :compile => directories do
  build_for = proc do |pik_ver, ver|
    sh %{ \
          c:\\devkit\\devkitvars.bat && \
          pik #{pik_ver} && \
          ruby extconf.rb && \
          make clean && \
          make && \
          cp *.so #{direc}/lib/#{ver} \
        }
  end
  
  chdir("#{direc}/ext/include_complete") do
    build_for.call("187", "1.8")
    build_for.call("192", "1.9")
  end
end

desc "build all platform gems at once"
task :gems => [:clean, :rmgems, "mingw32:gem", "mswin32:gem", "ruby:gem"]

desc "remove all platform gems"
task :rmgems => ["ruby:clobber_package"]

desc "build and push latest gems"
task :pushgems => :gems do
  chdir("#{direc}/pkg") do
    Dir["*.gem"].each do |gemfile|
      sh "gem push #{gemfile}"
    end
  end
end
