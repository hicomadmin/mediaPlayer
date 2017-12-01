cd ui
qmake

copy ..\core\debug\core.dll debug\
copy ..\core\debug\libcore.a .
make debug