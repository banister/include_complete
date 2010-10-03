# bring in user-defined extensions to TexPlay
direc = File.dirname(__FILE__)
dlext = Config::CONFIG['DLEXT']
begin
    if RUBY_VERSION && RUBY_VERSION =~ /1.9/
        require "#{direc}/1.9/real_include.#{dlext}"
    else
        require "#{direc}/1.8/real_include.#{dlext}"
    end
rescue LoadError => e
    require "#{direc}/real_include.#{dlext}"
end

require "#{direc}/real_include/version"
