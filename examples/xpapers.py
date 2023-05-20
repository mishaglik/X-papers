
import xpapers

xppr = xpapers.connect()

xppr.setBackgroundImageList(["Nyancat.gif", "Troll.jpg"])

xppr.loadPlugin("plugins/Clocks.so") # Loads plugin and simunaniously update python lib

clock = xppr.Clocks.addClock("Digit clock", .posX=middle, .posY=0)

if animation = xppr.loadAnimationFromShader("Aboba.gsls"):
  xppr.setBackroundImageChangeAnimation(animation)
else:
  xppr.setBackroundImageChangeAnimation(xppr.Animations[1])
xppr.setBackroundImageChangeTrigger(clock.every(60))

xppr.loadPlugin("plugins/Neofetch")
neo = xppr.Neofetch.addNeofetch("/* Args to neofetch */")
neo.setPosition(1000, 100)

label = xppr.CreateLabel(0, 100, 100, 10)
label.setFont("Fira Mono")
label.setTextStyle("Italic")
label.setText("CPU: {}%", xppr.getCpuLoad())
label.show()
