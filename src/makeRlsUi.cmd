cd ui
qmake

copy ..\core\release\core.dll release\
copy ..\core\release\libcore.a .

make release