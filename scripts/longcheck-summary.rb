#!/usr/bin/env ruby
#=========================================================================
# check-summary.rb [options] <build-dir>
#=========================================================================
#
#  -h --help          Display this message
#  -v --[no-]verbose  Verbose mode 
#
# Creates a summary of the cross-compiler test results. The given build
# directory option should point to the "src" build directory.
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
$opts[:build_dir] = "."
def parse_cmdline()
  OptionParser.new do |opts|
    opts.on("-v", "--[no-]verbose") { |v| $opts[:verbose] = v }
    opts.on("-h", "--help")         { usage() }
  end.parse!
  
  if ( !$ARGV.empty? )
    $opts[:build_dir] = $ARGV.pop()
  end
  $ARGV.empty? or raise OptionParser::InvalidOption
rescue
  usage()
end

#-------------------------------------------------------------------------
# Main
#-------------------------------------------------------------------------

def main()
  parse_cmdline()

  # Header

  puts '=' * 74
  puts "Cross-Compiler Test Results"
  puts '=' * 74

  # Info from Makefile

  makefile = "#{$opts[:build_dir]}/Makefile"
  if ( !File::exists?(makefile) )
    puts "\n ERROR: Given build directory does not include Makefile\n"
    exit(1);
  end  

  build   = ""
  host    = ""
  target  = ""
  cfg_arg = ""
  IO::foreach(makefile) do | line |
    build   = $1 if ( line =~ /^build=(.*)/ )
    host    = $1 if ( line =~ /^host=(.*)/ )
    target  = $1 if ( line =~ /^target=(.*)/ )
    cfg_arg = $1 if ( line =~ /^TOPLEVEL_CONFIGURE_ARGUMENTS=.*configure(.*)/ )
  end

  puts "build  = #{build}"
  puts "host   = #{host}"
  puts "target = #{target}"
  puts ""
  puts "configure arguments : #{cfg_arg.gsub(/--/,"\n  --")}"
  puts ""

  # Find the .sum files
  
  sum_files = [];
  Find::find($opts[:build_dir]) do | path |
    if ( File::extname(path) == ".sum" )
      sum_files.push(path)
    end
  end

  # Scan through .sum file for errors and summary

  for sum_file in sum_files.sort() do

    # Header for this file

    puts '-' * 74
    puts "#{File::basename(sum_file,".sum")} (#{sum_file})"
    puts '-' * 74
    puts ""

    error_found   = false
    summary_found = false
    IO::foreach(sum_file) do | line |

      # Errors

      if ( line =~ /^(FAIL|WARNING|XPASS|ERROR|UNRESOLVED)/ )
        error_found = true
        puts line
      end

      # Summary

      if ( line =~ /^\s+=== .* Summary ===/ )
        summary_found = true
        puts "" if error_found
      end

      if ( summary_found && ( line =~ /^# of (.*)\s+(\d+)$/ ) )
        puts " - %-20s = %6d" % [ $1.strip, $2 ]
      end

    end
    puts ""
  end

end
main()

