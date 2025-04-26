set -e

export DEVKITPRO=/opt/devkitpro
export RENPY_VER=8.3.7
export PYGAME_SDL2_VER=2.1.0

apt-get -y update
apt-get -y upgrade

apt -y install build-essential checkinstall
apt -y install libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev

apt -y install python3 python3-dev python3-pip

apt-get -y install p7zip-full libsdl2-dev libsdl2-image-dev libjpeg-dev libpng-dev libsdl2-ttf-dev libsdl2-mixer-dev libavformat-dev libfreetype6-dev libswscale-dev libglew-dev libfribidi-dev libavcodec-dev  libswresample-dev libsdl2-gfx-dev libgl1-mesa-glx
pip3 uninstall distribute
pip3 install future six typing requests ecdsa pefile Cython setuptools

python3 --version
