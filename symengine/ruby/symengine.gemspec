if Dir.exists?('symengine/ruby/')
    extconf_file = 'symengine/ruby/ext/symengine/extconf.rb'
else
    extconf_file = 'ext/symengine/extconf.rb'
end

Gem::Specification.new do |gem|
    gem.name = 'symengine'
    gem.version = '0.1.0'
    gem.summary = "Ruby interface for SymEngine, a fast C++ library for symbolic manipulation"
    gem.description = "This gem provides a Ruby interface for SymEngine, a fast C++ library for symbolic manipulation, so that the you can use the features of SymEngine in ruby."
    gem.authors = ['Abinash Meher']
    gem.email = ["abinashdakshana999@gmail.com"]
    gem.homepage = 'https://github.com/sympy/symengine'
    gem.files = Dir["*", "**/*", ".*", "**/.*"]
    gem.require_paths = ["lib"]
    gem.license = 'MIT'
    gem.extensions = [extconf_file]

    gem.add_development_dependency 'bundler', '~> 1.7'
    gem.add_development_dependency 'rspec', '~> 3.0'
    gem.add_development_dependency 'test-unit', '~> 3.1'
    gem.add_development_dependency 'rdoc', '~> 4.0'
end
