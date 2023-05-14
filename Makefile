all:
	make -C Drv
	make -C User/Viewer
	make -C User/Writer

clean:
	make -C Drv clean
	make -C User/Viewer clean
	make -C User/Writer clean

install:
	make -C Drv install

uninstall:
	make -C Drv uninstall
