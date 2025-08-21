/*
 *  The authors disclaim copyright to this source code
 */

function cb(id, event, err, data) {
	let d = document.getElementById("txt");
	if (id == "short") { 
		navigator.vibrate(30);
	} else if (id == "long") { 
		navigator.vibrate(100);
	} else if (id == "menu") {
	}
	d.innerText = d.innerText + id + " ";
	return true;
}

