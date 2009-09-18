#!/usr/bin/env ruby
#=========================================================================
# scheck-summary.rb [options] <test-out1> <test-out2> ...
#=========================================================================
#
#  -h --help          Display this message
#  -v --[no-]verbose  Verbose mode
#
# Creates a summary of the cross-compiler simple test results. Assumes
# .out files have -compile.out, -link.out, -run.out suffixes as
# appropriate.
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
$opts[:test_outs] = []
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
# check_out
#-------------------------------------------------------------------------

def check_out( test_out )

  if ( !FileTest::exist?(test_out) )
    return false;
  end
  
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
    return :passed
  else
    return :failed
  end

end

#-------------------------------------------------------------------------
# TestResults
#-------------------------------------------------------------------------

class TestResult
  attr_accessor :compile_result, :link_result, :run_result

  def initialize
    @compile_result = :unknown
    @link_result    = :unknown
    @run_result     = :unknown
  end

end

#-------------------------------------------------------------------------
# Main
#-------------------------------------------------------------------------

def main()
  parse_cmdline()

  # Check results of test cases

  tests   = []
  results = Hash::new
  for test_out in $opts[:test_outs] do

    case ( test_out )

      when /^(.*)-compile.out/ then
        test = "#{$1}"
        results[test] = TestResult::new if ( !results.has_key?(test) )
        tests.push(test) if ( !tests.include?(test) )
        results[test].compile_result = check_out(test_out)

      when /^(.*)-link.out/ then
        test = "#{$1}"
        results[test] = TestResult::new if ( !results.has_key?(test) )
        tests.push(test) if ( !tests.include?(test) )
        results[test].link_result = check_out(test_out)

      when /^(.*)-run.out/ then
        test = "#{$1}"
        results[test] = TestResult::new if ( !results.has_key?(test) )
        tests.push(test) if ( !tests.include?(test) )
        results[test].run_result = check_out(test_out)

    end
  
  end

  # Cascade failures from compile to link to run

  for test in tests do

    # If compiled fails then link and run fail
    if ( results[test].compile_result == :failed )

      if ( results[test].link_result == :passed )
        results[test].link_result == :failed
      end

      if ( results[test].run_result == :passed )
        results[test].run_result == :failed
      end

    # if link fails then run fails
    elsif ( results[test].link_result == :failed )

      if ( results[test].run_result == :passed )
        results[test].run_result == :failed
      end

    end

  end

  # Display results of test cases

  any_fail = false
  test_module = ""
  puts ""
  for test in tests do

    plist = []
    flist = []

    test =~ /test-([^-]+)-.*/
    if ( $1 != test_module )
      test_module = $1
      puts "" if ( any_fail )
      any_fail = false
      puts " Test Module : #{test_module}"
    end

    case ( results[test].compile_result )
      when :passed then plist.push("compile")
      when :failed then flist.push("compile")
    end

    case ( results[test].link_result )
      when :passed then plist.push("link")
      when :failed then flist.push("link")
    end

    case ( results[test].run_result )
      when :passed then plist.push("run")
      when :failed then flist.push("run")
    end

    # Output final summary

    # if ( !plist.empty? )
    #   puts "  [ passed ] #{test} (#{plist.join(",")})"
    # end

    if ( !flist.empty? )
      puts "  [ FAILED ] #{test} (#{flist.join(",")})"
      any_fail = true
    end

  end
  puts ""

end
main()

