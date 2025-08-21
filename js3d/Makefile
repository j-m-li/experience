
all:
	node cube.js > model.stl
	cat model.stl
	admesh -v -b model_bin.stl model.stl
	admeshgui model.stl

clean:
	rm -f *.stl

