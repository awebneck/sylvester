require 'rubygems'
require 'spork'
require 'pry'

#uncomment the following line to use spork with the debugger
#require 'spork/ext/ruby-debug'

Spork.prefork do
  $LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))
  $LOAD_PATH.unshift(File.dirname(__FILE__))
  require 'rspec'
  require 'sylvester'

  Dir["#{File.dirname(__FILE__)}/support/**/*.rb"].each {|f| require f}

  RSpec.configure do |config|
  end
end

Spork.each_run do
  Dir["#{File.dirname(__FILE__)}/../lib/**/*.rb"].each {|f| load f }
  require "#{File.dirname(__FILE__)}/../lib/sylvester.so"
end
