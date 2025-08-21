
all:
	node toamxd.js -m source/m.json  devices/m.amxd
	node toamxd.js -a source/a.json  devices/a.amxd
	node toamxd.js -i source/i.json  devices/i.amxd
	node asd2json.js media/untitled2.wav.asd media/u.json
	type media\u.json || cat media/u.json
	node asd2json.js media/m.wav.asd media/mu.json
	type media\mu.json || cat media/mu.json

