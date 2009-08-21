#!/usr/bin/env ruby
#=========================================================================
# scheck-summary.rb [options] <test-out1> <test-out2> ...
#=========================================================================
#
#  -h --help          Display this message
#  -v --[no-]verbose  Verbose mode 
#
# Creates a summary of the cross-compiler simple test results. The test
# module and a list of test outputs should be supplied on the command
# line.
#
# Author : Christopher Batten
# Date   : July 31, 2009
# 

require 'optparse'
require 'rdoc/usage'
require 'stringio'
require 'find'

# Enable ruby warnings (this avoid problems with "ruby -w")
$VERBOSE = true

#-------------------------------------------------------------------------
# Command line processing
#-------------------------------------------------------------------------

def usage()
  $stdout = StringIO::new
  RDoc::usage_no_exit
  STDOUT.puts($stdout.string.gsub(/\A=+\n(.*)\n\n=+/,"\n\\1\n"))
  exit(1)
end

$opts = {}
$opts[:test_outs]   = []
def parse_cmdline()
  OptionParser.new do |opts|
    opts.on("-v", "--[no-]verbose") { |v| $opts[:verbose] = v }
    opts.on("-h", "--help")         { usage() }
  end.parse!
  $opts[:test_outs] = $ARGV
rescue
  usage()
end

#-------------------------------------------------------------------------
# Main
#-------------------------------------------------------------------------

def main()
  parse_cmdline()

  puts ""
  for test_out in $opts[:test_outs] do

    found  = false
    passed = true
    IO::foreach(test_out) do | line | 
      if ( line =~ /^\*\*\* .* exit = (\d+)$/ )
        found = true
        if ( $1.to_i > 0 )
          passed = false
        end
      end
    end

    if ( found && passed )
      puts " [ passed ] #{test_out}"
    else
      puts " [ FAILED ] #{test_out}"
    end

  end
  puts ""

end
main()

