import xpapers

xpap = xpapers.connect()

xpap.loadModule("bg")
bg = xpap.bg.add(0)
bg.setImage("/home/gms/I1.jpg")