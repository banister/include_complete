$LOAD_PATH.unshift File.join(File.expand_path(__FILE__), '..')

require 'rake/clean'
require 'rake/gempackagetask'

# get the texplay version
require 'lib/real_include/version'

$dlext = Config::CONFIG['DLEXT']

CLEAN.include("ext/**/*.#{$dlext}", "ext/**/*.log", "ext/**/*.o", "ext/**/*~", "ext/**/*#*", "ext/**/*.obj", "ext/**/*.def", "ext/**/*.pdb")
CLOBBER.include("**/*.#{$dlext}", "**/*~", "**/*#*", "**/*.log", "**/*.o")

specification = Gem::Specification.new do |s|
    s.name = "real_include"
    s.summary = "Fixing the limitation in traditional Module#include"
    s.version = RealInclude::VERSION
    s.date = Time.now.strftime '%Y-%m-%d'
    s.author = "John Mair (banisterfiend)"
    s.email = 'jrmair@gmail.com'
    s.description = s.summary
    s.require_path = 'lib'
    s.platform = Gem::Platform::RUBY
    s.homepage = "http://banisterfiend.wordpress.com"
    s.has_rdoc = false

    s.extensions = ["ext/real_include/extconf.rb"]
    s.files =  ["Rakefile", "README.markdown", "CHANGELOG", 
                "lib/real_include.rb", "lib/real_include/version.rb"] +
        FileList["ext/**/extconf.rb", "ext/**/*.h", "ext/**/*.c"].to_a 
end

