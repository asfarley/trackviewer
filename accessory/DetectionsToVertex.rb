require 'json'

detections_path = ARGV[0]
vertex_path = File.dirname(detections_path) + "\\vertex.txt"

z = 0
File.open(vertex_path, 'a') do |file|
	File.readlines(detections_path).each do |line|
		detectionsJson = JSON.parse(line)
		detectionsArray = detectionsJson["Measurements"]
		detectionsArray.each do |d|
				x = d["X"]
				y = d["Y"]
				r = d["Red"]
				g = d["Green"]
				b = d["Blue"]
				cl = d["ObjectClass"]
				size = d["Size"]
				width = d["Width"]
				height = d["Height"]
				
				#puts "x:#{x} y:#{y} r:#{r} g:#{g} b:#{b} class:#{cl} size:#{size} width:#{width} height:#{height}"
				
				file.puts "#{x} #{y} #{z} #{r} #{g} #{b}"
		end
		z += 1
	end
end