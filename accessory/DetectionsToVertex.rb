detections_path = ARGV[0]
vertex_path = File.dirname(detections_path) + "\\vertex.txt"

z = 0
File.open(vertex_path, 'a') do |file|
	File.readlines(detections_path).each do |line|
		detections = line.split(',')
		detections.each do |d|
			if d.length > 1
				properties = d.split(' ')
				x = properties[1].tr('[]:', '')
				y = properties[3].tr('[]:', '')
				r = properties[5].tr('[]:', '')
				g = properties[7].tr('[]:', '')
				b = properties[9].tr('[]:', '')
				cl = properties[11].tr('[]:', '')
				size = properties[13].tr('[]:', '')
				width = properties[15].tr('[]:', '')
				height = properties[17].tr('[]:', '')
				
				#puts "x:#{x} y:#{y} r:#{r} g:#{g} b:#{b} class:#{cl} size:#{size} width:#{width} height:#{height}"
				
				file.puts "#{x} #{y} #{z} #{r} #{g} #{b}"
			end
		end
		z += 1
	end
end