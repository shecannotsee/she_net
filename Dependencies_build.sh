git submodule add https://github.com/shecannotsee/sheLog.git third_party/sheLog
git submodule add https://github.com/shecannotsee/sheIni.git third_party/sheIni

mkdir libraries

cd third_party/sheIni/
mkdir build
cd build
cmake ..
make -j8
make install DESTDIT=./
mv -f ./sheIni ../../../libraries/

cd ../../
cd sheLog/
mkdir build
cd build
cmake ..
make -j8
make install DESTDIT=./
mv -f ./she_log ../../../libraries/
