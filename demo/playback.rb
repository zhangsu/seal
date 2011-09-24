#coding: GBK

require './seal'
include Audio

Audio.startup

src = Source.new
src.buffer = Buffer.new("D:/music/无锡 太湖美 程桂兰.mp3")
src.play
sleep(10)

Audio.cleanup