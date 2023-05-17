import xpapers

xpap = xpapers.connect()

xpap.loadModule("bg")
bg = xpap.bg.add(0)
bg.addImage("/home/gms/I2.jpg")

import os
xpap.loadModule("build/src/Plugins/Widget/Label/liblabel.so")
label = xpap.label.add(0)
label.setText("Hello, text!")
label.setColor(0xFFFFFFFF)
label.setPos(10, 30)
label.setFont("/usr/share/fonts/TTF/OpenSans-Bold.ttf")
label.setSize(30)
