if [ ! -d bin ] # 判断是否有include这个目录
then
    mkdir bin
fi

if [ ! -d build ] # 判断是否有include这个目录
then
    mkdir build
fi

cd build
cmake ..
make