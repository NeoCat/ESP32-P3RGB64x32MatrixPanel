#!/usr/bin/env ruby
require 'mini_magick'
require 'socket'

if ARGV.size != 2
  STDERR.puts "usage: #{$PROGRAM_NAME} ip_address_of_ESP32 image_file_path"
  exit 1
end

image = MiniMagick::Image.open ARGV[1]
new_height = image.height * 64 / image.width
image.resize "64x#{new_height}!"
puts "#{image.width}x#{image.height}"
image.contrast
image.gamma 0.3
pixels = image.get_pixels

sock = TCPSocket.open(ARGV[0], 20032)

([new_height - 31, 1].max).times do |y0|
  pix = []
  32.times do |y|
    64.times do |x|
      r, g, b = pixels[[y + y0, image.height - 1].min][x].map { |v| v >> 3 }
      c = r | (g << 5) | (b << 10)
      pix << c
    end
  end
  data = pix.pack('v*')
  sock.write data
  sleep 0.1
  sleep 0.5 if y0 == 0
end
