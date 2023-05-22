import xpapers

xpap = xpapers.connect()

# xpap.loadModule("bg")
# bg = xpap.bg.add(0)
# bg.addImages(["/home/gms/WP1.jpg", "/home/gms/WP2.jpg"])

xpap.loadModule("build/src/Plugins/Widget/Movie/libmovie.so")
movie = xpap.movie.add(0)
movie.setVideoPath("/home/gms/wallpaper.mp4")
movie.fit(0, 0, 1920, 1080)

xpap.loadModule("build/src/Plugins/Widget/BaseWidgets/libbaseWigets.so")
label = xpap.wid.addBorderedLabel(0)
label.setText("Hello, text!")
label.setTextColor(0xFFB3E0FF)
label.setBorderColor(0xFF0000FF)
label.setPos(20, 100)
label.setFont("/usr/share/fonts/TTF/OpenSans-Bold.ttf")
label.setSize(30)
