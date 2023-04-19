
import xpapers

xppr = xpapers.connect()

xppr.setBackgroundImageList(["Nyancat.gif", "Troll.jpg"])

xppr.loadPlugin("plugins/Clocks.so") # Loads plugin and simunaniously update python lib

clock = xppr.Clocks.addClock("Digit clock", .posX=middle, .posY=0)

xppr.setBackroundImageChangeAnimation("Animation1")
xppr.setBackroundImageChangeTrigger(clock.every(60))

xppr.loadPlugin("plugins/Neofetch")
xppr.Neofetch.addNeofetch("/* Args to neofetch */")

xppr.printText("Hello, world!", 0, 100, 100, 10, "Fira Mono")