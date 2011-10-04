#!/usr/bin/env ruby
answer = (1..5000).map { |t|
  begin
    fname = "%04d.txt" % t
    File.open(fname,'r'){ |f| 
      f.read.split("\n").last
    }
  rescue
    ''
  end
}

a_file = 'answer.txt'

File.open(a_file,'w'){ |f|
  answer.each { |l|
    f.puts l
  }
}

l,r,u,d = 0,0,0,0
File.open(a_file,'r'){ |f|
  f.read
}.each_line { |line|
  l += line.count("L")
  r += line.count "R"
  u += line.count "U"
  d += line.count "D"
}

puts "L:#{l} R:#{r} U:#{u} D:#{d}"

