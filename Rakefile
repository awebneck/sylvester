# encoding: utf-8

require 'rubygems'
require 'bundler'
require 'pry'

begin
  Bundler.setup(:default, :development)
rescue Bundler::BundlerError => e
  $stderr.puts e.message
  $stderr.puts "Run `bundle install` to install missing gems"
  exit e.status_code
end
require 'rake'

require 'jeweler'
Jeweler::Tasks.new do |gem|
  # gem is a Gem::Specification... see http://docs.rubygems.org/read/chapter/20 for more options
  gem.name = "sylvester"
  gem.homepage = "http://github.com/awebneck/sylvester"
  gem.license = "MIT"
  gem.summary = %Q{A ruby library for interacting with, storing, and reading basic ruby objects from System V shared memory}
  gem.description = %Q{The beginnings of a handy ruby interface for System V shared memory, making it accessible and easy to use directly from ruby. Plans are to simply treat it as a string to begin with, thereafter building more complex data structures that will implement the interfaces (where appropriate) of their stack-based counterparts.}
  gem.email = "jeremy@jeremypholland.com"
  gem.authors = ["Jeremy Holland"]
  # dependencies defined in Gemfile
end
Jeweler::RubygemsDotOrgTasks.new

require 'rspec/core'
require 'rspec/core/rake_task'
RSpec::Core::RakeTask.new(:spec) do |spec|
  spec.pattern = FileList['spec/**/*_spec.rb']
end

RSpec::Core::RakeTask.new(:rcov) do |spec|
  spec.pattern = 'spec/**/*_spec.rb'
  spec.rcov = true
end

task :default => :spec

require 'rdoc/task'
Rake::RDocTask.new do |rdoc|
  version = File.exist?('VERSION') ? File.read('VERSION') : ""

  rdoc.rdoc_dir = 'rdoc'
  rdoc.title = "sylvester #{version}"
  rdoc.rdoc_files.include('README*')
  rdoc.rdoc_files.include('lib/**/*.rb')
end

desc "Compile for testing"
task :buildext do
  puts `/usr/bin/env ruby #{File.dirname(__FILE__)}/ext/sylvester/extconf.rb`
  puts `/usr/bin/make`
  puts `rm *.o > /dev/null 2>&1`
  puts `rm mkmf.log > /dev/null 2>&1`
  puts `rm Makefile > /dev/null 2>&1`
  puts `mkdir -p #{File.dirname(__FILE__)}/spec/build/sylvester > /dev/null 2>&1`
  Kernel.exec "mv sylvester.{bundle,so} #{File.dirname(__FILE__)}/lib > /dev/null 2>&1"
end
