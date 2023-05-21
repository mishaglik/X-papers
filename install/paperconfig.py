import xpapers

xpap = xpapers.connect()

xpap.loadModule("bg")
bg = xpap.bg.add(0)
bg.addImages(["/home/execorn/wallpapers/imgs/fuji.jpg", "/home/execorn/wallpapers/imgs/fuji_2.jpg"])

import os
xpap.loadModule("build/src/Plugins/Widget/BaseWidgets/libbaseWigets.so")
label = xpap.wid.addBorderedLabel(0)
label.setText("Hello, text!")
label.setTextColor(0xFFB3E0FF)
label.setBorderColor(0xFF0000FF)
label.setPos(20, 100)
label.setFont("/usr/share/fonts/TTF/OpenSans-Bold.ttf")
label.setSize(30)
