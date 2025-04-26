# Keyboard Cat

A playful desktop widget that shows an animated cat reacting to your keyboard activity.

![Keyboard Cat Demo](./resources/hallow_cat.gif)

## Build

#### Conan
I RECOMMENDED use Conan because it's really easy


```sh
conan install . --build=missing --output-folder=build
conan build .
```

#### Linux (Debian/Ubuntu)
```sh
sudo apt-get install -y \
    build-essential \
    cmake \
    libsdl3-dev \
    libgif-dev \
    libavcodec-dev \
    libavformat-dev \
    libswscale-dev \
    libsdl3-image-dev \
    libsdl3-ttf-dev \
    libsdl3-mixer-dev
```

#### Linux (Arch/Manjaro)
```sh
sudo pacman -S --needed \
    base-devel \
    cmake \
    sdl3 \
    sdl3_image \
    sdl3_ttf \
    sdl3_mixer \
    giflib
```

#### macOS (Homebrew)
```sh
brew install \
    cmake \
    sdl3 \
    sdl3_image \
    sdl3_ttf \
    sdl3_mixer \
    giflib
```

## How to Use:
Just run executable
