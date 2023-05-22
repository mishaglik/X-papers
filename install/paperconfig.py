import xpapers

xpap = xpapers.connect()

import os

xpap.loadModule("build/src/Plugins/Widget/Movie/libmovie.so")
movie = xpap.movie.add(0)
movie.setVideoPath("/home/execorn/Downloads/wallpaper.mp4")
movie.fit(0, 0, 1920, 1080)
