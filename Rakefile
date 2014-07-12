require "bundler/gem_tasks"
require "rspec/core/rake_task"
require "rake/extensiontask"

require 'rake/extensiontask'
Rake::ExtensionTask.new 'gurobi' do |ext|
  ext.lib_dir = 'lib/gurobi'
end

RSpec::Core::RakeTask.new(:spec)

task :default => :spec

require 'yard'
YARD::Rake::YardocTask.new do |t|
  t.files   = ['lib/**/*.rb', 'lib_for_doc/**/*.rb', 'ext/gurobi/gurobi.cpp']
 #t.files   = ['ext/**/*.cpp']
  t.options = %w(--no-cache)
end
