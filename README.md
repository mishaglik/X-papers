# X-Papers

# 1. What is this project all about?
This project tries to reproduce the background render capabilities of Wallpaper Engine on Linux systems. It's as simple as that. It allows you to render both image and video wallpapers with high efficiency and works with all popular Window Managers.
### What is the original Wallpaper Engine?
Original [Wallpaper Engine](https://store.steampowered.com/app/431960/Wallpaper_Engine/) is a proprietary application for Windows with a companion app on Android which allows users to use and create animated and interactive wallpapers. 
This project provides all the same functionality with the exception of steam workshop integration.

# 2. Compilation requirements
## X-papers
- OpenGL 2.1 support
- CMake
- SDL
- sfeMovie
- FFmpeg
- X11
- Xrandr
- GLFW3
- GLM
- MPV
- Python3.10+
- PulseAudio

# 3. How to use X-papers?
## 4.1. Pre-requirements
In order to properly use this software you'll need to install all packages required for compiling the project and get yourself a video-file for rendering.

The assets themselves **must** be copied to the same folder where the binary lives.

## 3.3. Getting source code
Download a zipped version of the repository here: https://github.com/mishaglik/X-papers

You can also clone the repository using git like this:
```
git clone https://github.com/mishaglik/X-papers.git
```

Or using the HTTPS method if you have set up SSH:
```
https://github.com/mishaglik/X-papers.git
```

## 3.4. Compilation steps
The project is built on CMake as the build engine. First we need to create the directory where the build will be stored and get into it:

```
mkdir build
cd build
```

Once the folder is created and we're in it, CMake has to generate the actual Makefiles. This can be done this way:
```
cmake ..
```
Take a closer look at the CMake output, if you miss any library, CMake will report the missing libraries so you can install them either trough your package manager or manually in your system.

Finally we can compile the project to generate the actual executable. 
```
make
```

# 4. Running a background

## Main configuration is made using python scripts with just a few steps:
1) Create python script placed in folder **install**
2) Add lines in the beginning:
3) ```import xpapers```
4) ```xpap = xpapers.connect # connect with server```
5) To load your mode you need to load your module in such way
6) ```xpap.loadModule("/your/module/name")```
7) After the module is loaded, use it's API to render whatever you want on the background.


# 5. Example of simple script
## Here is the example of a configuration that renders background with a label "Meow, world!" on it.
```
# import the main module that allows to render the background
import xpapers
# create display connection
xpap = xpapers.connect()

# load module 'Movie' (you can load any other module if you want)
xpap.loadModule("build/src/Plugins/Widget/Movie/libmovie.so")

# using module 'Movie' API: add movie widget on the background to the monitor with index 0
movie = xpap.movie.add(0)

# set video path for your movie
movie.setVideoPath("/home/execorn/Downloads/meow.mp4")

# set the size of your video widget, and it's coordinates
# note: xpapers support python arrays as well
movie.fit(0, 0, 1920, 1080)

# load module 'baseWidgets' that provides the simplest widgets out of the box
xpap.loadModule("build/src/Plugins/Widget/BaseWidgets/libbaseWigets.so")
# create new bordered label or the screen with index 0
label = xpap.wid.addBorderedLabel(0)
# it does exactly what you think it does
label.setText("Meow, World!")
label.setTextColor(0xFFB3E0FF)

# set label parameters
label.setBorderColor(0xFF0000FF)

# set label position (0,0) point is on the top left of the screen
label.setPos(20, 100)

# open font file and set font for the label 
label.setFont("/usr/share/fonts/TTF/OpenSans-Bold.ttf")
# set text size
label.setSize(27)
```

# 6. Command line arguments
## To get help do ```xpapers --help```
Setup your config file to ```.config/xpapers.py```
Usage ```xpapers [-d| --daemonize] [--fps] [--vs] [--help]```
1) ```-d | --daemonize``` - daemonize X-papers
2) ```--fps``` - set framerate limit
3) ```--vs``` - enable Vertical Synchronization for rendering


# 7. Absraction's performance
## Comparison of zero-cost compile-time constructions and run-time abstractions
Testing:
1) Creating 10000 labels
2) 1. Compile time: $\approx 80 \pm 3$ ms/frame
   2. Runtime: $\approx 95 \pm 3$  ms/frame

### Result: 
Runtime abstraction caused $\approx 15 \\%$ slowdown 

## Examples of abstractions.
This is example of poor and proper abstractions usage:
### Bad version (Using run-time virtual function calls)
```C++
   struct BaseWidget {
        /* ... */
        virtual void draw(RenderWindow& window) = 0;
        /* ... */
   };

    struct Bordered  : BaseWidget{
        Bordered(BaseWidget* widget) : m_widget(widget) {
            /* Do some job...*/
        }

        virtual void draw(RenderWindow& window) override {
            m_widget->draw(window);
            drawBox();
        }

        void drawBox() {
            /*Drawing box logic*/
        }

        BaseWidget* m_widget;
    };
``` 

### Good version (Using template-based compile-time polymorphism)
```C++
    template<Widget W>
    struct Bordered : public W {
        Bordered(W widget) : W(std::move(widget)) {
            /* Do some job...*/
        }

        void draw(RenderWindow& window) override {
            W::draw(window); // Here compiler will iniline this function. So we purely safe one virtual call. 
            drawBox();
        }

        void drawBox() {
            /*Drawing box logic*/
        }
    };
``` 
and with some template wrapping this code will become into: 
```C++
auto* wid = Label() | Bordered(0x00FF00FF) | OnSprite(400, 100);
```



# 8 Features
## Plugins included in X-papers
1) Movie plugin
2) baseWidgets plugin (label / rectangle **with compile-time decorators**)
3) python as a plugin (xpapers)

## Advanced capabilities
1) X-papers has it's own dynamic system types
2) Templated conversion of static-typed code into dynamic-typed code
3) Extremely simple XradnR API that provides monitors and X-displays enumeration
4) Fast renderer based on ffmpeg and openGL

## Demonstration gif
![sample](examples/example.gif)
