def execute_binary(command)
  `#{File.dirname(__FILE__)}/bin/#{command} > /dev/null & echo $!`.to_i
end

def kill_binary(pid)
  `kill #{pid}`
end
