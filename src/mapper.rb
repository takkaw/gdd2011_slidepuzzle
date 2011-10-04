#!/usr/bin/env ruby
# coding : utf-8
Version = '0.0.3'

require 'optparse'
require 'pathname'
require 'timeout'
require 'benchmark'
TIMEOUT = 60
PATH = Pathname(__FILE__)

LIMITS,BOARDS,PROBLEMS = File.open("problems.txt","r") { |f|
  [f.gets,f.gets,f.read.split("\n")]
}

def no_path(no)
  ( Pathname(__FILE__) + "../../answer/%04d.txt" % no ).to_s
end

def solve_no(no,out)
  x,y,problem = PROBLEMS[no-1].split(',')
  pid = false
  begin
    timeout(TIMEOUT){
      pid = spawn("./solve #{x} #{y} #{problem}",:out => out ? STDOUT : no_path(no))
      Process.wait pid
    }
    puts "solve! No.%04d" % no
  rescue Timeout::Error
    puts "Timeout Skip No.%04d..." % no
    Process.kill 'KILL',pid
    Process.wait pid
    File.delete "../answer/%04d.txt" % no unless out
    File.open('ignore.txt','a'){ |f|
      f.write "%04d  # #{x}x#{y} timeout #{TIMEOUT}\n" % no
    }
  end
end

def file_exist?(no,flg)
  file = no_path(no)

  exist = Kernel.test(?e,file)
  if exist && flg
    File.delete file
    exist = false
  end
  return exist
end

IGNORE = File.open("ignore.txt","r"){ |f|
  f.read
}.split("\n").map{|t| t.split('#').first.to_i}

def ignore_list?(no)
  IGNORE.include? no.to_i
end

force_update = false
mode = false
no = false
x = false
y = false
out = false

op = OptionParser.new {|opt|
  opt.on('-n = No','--no = No','solve selected no.') { |v|
    no = v.to_i
    mode = :no
  }
  opt.on('-m = MxN','--matrix = MxN','solve selected matrix. ex) 3x3') { |v|
    x,y = v.split('x')
    mode = :matrix
  }
  opt.on('-f','--force','solve even if solved.'){ |v| force_update = v }
  opt.on('-d','--debug','output to stdout'){ |v| out = v }
}
op.parse!(ARGV)

unless mode
  puts op.help
  exit
end

case mode
when :no
  puts "Try No.%04d..." % no
  puts Benchmark::CAPTION
  puts Benchmark.measure {
    solve_no(no,out) unless file_exist?(no,force_update)
  }
when :matrix
  PROBLEMS.each.with_index { |problem,i|
    _x,_y,board = problem.split(',')
    if x == _x && y == _y
      no = i + 1
      if ignore_list? no
        puts "Skip No.%04d..." % no
        next
      end
      if file_exist?(no,force_update)
        puts "Done No.%04d" % no
        next
      else
        puts "Try! No.%04d..." % no
        solve_no(no,out) 
      end
    end
  }
end


