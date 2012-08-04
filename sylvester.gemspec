# Generated by jeweler
# DO NOT EDIT THIS FILE DIRECTLY
# Instead, edit Jeweler::Tasks in Rakefile, and run 'rake gemspec'
# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "sylvester"
  s.version = "0.0.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Jeremy Holland"]
  s.date = "2012-08-04"
  s.description = "The beginnings of a handy ruby interface for System V shared memory, making it accessible and easy to use directly from ruby. Plans are to simply treat it as a string to begin with, thereafter building more complex data structures that will implement the interfaces (where appropriate) of their stack-based counterparts."
  s.email = "jeremy@jeremypholland.com"
  s.extensions = ["ext/sylvester/extconf.rb"]
  s.extra_rdoc_files = [
    "LICENSE.txt",
    "README.rdoc"
  ]
  s.files = [
    ".document",
    ".rspec",
    "Gemfile",
    "Gemfile.lock",
    "LICENSE.txt",
    "README.rdoc",
    "Rakefile",
    "VERSION",
    "ext/sylvester/extconf.rb",
    "ext/sylvester/sylvester.c",
    "ext/sylvester/sylvester.h",
    "ext/sylvester/sylvester_errors.c",
    "ext/sylvester/sylvester_errors.h",
    "ext/sylvester/sylvester_share.c",
    "ext/sylvester/sylvester_share.h",
    "spec/spec_helper.rb",
    "spec/support/bin/attach",
    "spec/support/bin_commands.rb",
    "spec/support/bin_helper.rb",
    "spec/support/test_shares.rb",
    "spec/sylvester/share_spec.rb",
    "spec/sylvester_spec.rb"
  ]
  s.homepage = "http://github.com/awebneck/sylvester"
  s.licenses = ["MIT"]
  s.require_paths = ["lib"]
  s.rubygems_version = "1.8.15"
  s.summary = "A ruby library for interacting with, storing, and reading basic ruby objects from System V shared memory"

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<rspec>, ["~> 2.8.0"])
      s.add_development_dependency(%q<rdoc>, ["~> 3.12"])
      s.add_development_dependency(%q<bundler>, ["~> 1.1.0"])
      s.add_development_dependency(%q<jeweler>, ["~> 1.8.4"])
      s.add_development_dependency(%q<simplecov>, ["~> 0.6.4"])
      s.add_development_dependency(%q<spork>, ["~> 0.9.2"])
      s.add_development_dependency(%q<pry>, [">= 0"])
    else
      s.add_dependency(%q<rspec>, ["~> 2.8.0"])
      s.add_dependency(%q<rdoc>, ["~> 3.12"])
      s.add_dependency(%q<bundler>, ["~> 1.1.0"])
      s.add_dependency(%q<jeweler>, ["~> 1.8.4"])
      s.add_dependency(%q<simplecov>, ["~> 0.6.4"])
      s.add_dependency(%q<spork>, ["~> 0.9.2"])
      s.add_dependency(%q<pry>, [">= 0"])
    end
  else
    s.add_dependency(%q<rspec>, ["~> 2.8.0"])
    s.add_dependency(%q<rdoc>, ["~> 3.12"])
    s.add_dependency(%q<bundler>, ["~> 1.1.0"])
    s.add_dependency(%q<jeweler>, ["~> 1.8.4"])
    s.add_dependency(%q<simplecov>, ["~> 0.6.4"])
    s.add_dependency(%q<spork>, ["~> 0.9.2"])
    s.add_dependency(%q<pry>, [">= 0"])
  end
end

