# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'gurobi/version'

Gem::Specification.new do |spec|
  spec.name          = "gurobi"
  spec.version       = Gurobi::VERSION
  spec.authors       = ["Fuminori Ido"]
  spec.email         = ["ido00 at sourceforge.net"]
  spec.summary       = %q{Gurobi Ruby binding}
  spec.description   = %q{Gurobi Ruby binding}
  spec.homepage      = ""
  spec.license       = "MIT"
  spec.extensions    = %w[ext/gurobi/extconf.rb]

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.6"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rspec"
end
