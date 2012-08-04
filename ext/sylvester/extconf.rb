require "mkmf"

HEADER_DIRS = [
  # First search /opt/local for macports
  '/opt/local/include',
  # Then search /usr/local for people that installed from source or homebrew
  '/usr/local/include',
  # Finally fall back to /usr
  '/usr/include',
]

LIB_DIRS = [
  # First search /opt/local for macports
  '/opt/local/lib',
  # Then search /usr/local for people that installed from source or homebrew
  '/usr/local/lib',
  # Finally fall back to /usr
  '/usr/lib',
]

headers = [
  "stdarg.h",
  "time.h",
  "sys/types.h",
  "sys/ipc.h",
  "sys/shm.h",
]

libraries = [
]

puts ">> check require libraries..."

libraries.each{|lib|
  raise "lib#{lib} not found." unless have_library(lib)
}

puts ">> check require headers..."
headers.each{|header|
  raise "#{header} not found." unless have_header(header)
}

create_makefile("sylvester/sylvester")
