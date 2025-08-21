const fs = require('fs');

console.log("hello");

let f = fs.readFileSync(process.argv[2]);
let h = new Uint8Array(f);

switch (process.argv[2]) {
case "-h":
	break;
}

function dbl(o)
{
	return int32(o);
}

function int32(o)
{
	return (h[o] + (h[o+1]<<8) + (h[o+2]<<16) + (h[o+3]<<24));
}

function utf16(o, l)
{
	let i;
	let d = "";
	let s = int32(o);
	if (s > l) {
		console.log("utf16 len " + s);
		return [0,0];
	}
	o += 4;
	for (i = 0; i < s; i += 1) {
		d += String.fromCodePoint(h[o+i*2]);
	}
	return [d,s*2+4];
}

function ascii(o, l)
{
	let i;
	let d = "";
	let s = h[o];
	if (s > l) {
		console.log("ascii len " + s);
		return [0,0];
	}
	o++;
	for (i = 0; i < s; i += 1) {
		d += String.fromCodePoint(h[o+i]);
	}
	return [d,s+1];
}

function OnsetEvent(o, l, n)
{
	let i;
	let d = "";
	let s = o;
	let r;

	d += "[\n";
	for (i = 0; i < n; i++) {
		r = utf16(o,l);
		if (i > 0) {
			d += ",";
		}
		o += r[1];
		d += "\n{\"" + r[0] + "\": " + h[o] + " ";
		o++;
	}
	d += "]\n";
	return [d,o-s];
}

let auf = 0;
function AufTaktData(o, l, n)
{
	let i;
	let d = "";
	let s = o;
	let r = [];
	auf++;
	if (auf == 3) {
		d += " AOOOO " + auf;
		o -= 0;
		//return r;
	} else if (auf == 2) {
		r[1] = 0;
		r[0] = " AOOOO " + auf;
		return r;
	}
	d += "[\n";
	//o -= 4;
	for (i = 0; i < n; i++) {
		r = utf16(o,l);
		if (i > 0) {
			d += ",";
		}
		console.log(i + "KKK " +r[0]);
		o += r[1];
		d += "\n{\"" + r[0] + "\": " + h[o] + " ";
		o++;
	}
	if (auf == 3) {
		i = int32(o);
		d += " SHIIII " + i;
		o += 4 + i + 13;
	}
	d += "]\n";
	return [d,o-s];
}


function SampleOverView(o, l, n)
{
	let i;
	let d = "";
	let s = o;
	let r;

	d += "[\n";
	r = utf16(o,l);
	o += r[1];
	d += "\n{\"" + r[0] + "\": " + h[o] + " ";
	o++;
	r = ascii(o,l);
	o += r[1];
	d += ",\n{\"" + r[0] + "\": " + h[o] + " ";
	r = utf16(o,l);
	o += r[1];
	d += ",\n{\"" + r[0] + "\": " + h[o] + " ";
	o++;
	r = utf16(o,l);
	o += r[1];
	d += ",\n{\"" + r[0] + "\": " + h[o] + " ";
	o++;
	r = utf16(o,l);
	o += r[1];
	d += ",\n{\"" + r[0] + "\": " + h[o] + " ";
	o++;



	d += "]\n";
	return [d,o-s];
}

var samplelevel = 0;
function SampleOverViewLevel(o, l, n)
{
	let i;
	let d = "";
	let s = o;
	let r = [];
	let x;
	let dat;
	samplelevel++;
	if (samplelevel >= 2) {
		console.log("OOOOOOOOOOOO " + int32(o));
		r[1] = int32(o) * 2 + 4;// 27;
		r[0] = "";
		console.log("NNNN" + (s+r[1]));
		return r;
	}
	d += "[\n";
	r = utf16(o,l);
	o += r[1];
	d += "\n{\"" + r[0] + "\": " + h[o] + " ";
	x = h[o] * 3; 
	o += 1;
	d += "MMMMM " + x + " " + o + "\n";
	while (x > 0) {
		d += ", "  + x;
		while (x > 10) {
			d += ", "  + h[o];
			o++;
			x--;
		}
		break;
	}
	dat = int32(o) & 0xFFFF;
		d += "i!! "  + dat + " !! ";
	o += x;
	o += dat * 8;
	o++;
	o++;
	o++;

	o += 4;
	d += " "  + o + " ]\n";
	return [d,o-s];
}





let o = 0;
let d = "{";
d += "\"I\":[" ;
o = 2;
let n = int32(o);
let r;
d += n;
o = 6;
d += "," + int32(o);
o = 10;
d += "," + int32(o);
d += "]\n";

o = 34 + n * 4;
n = h.length - o;
o += 3;
r = ascii(o, n);
d += ",\"" + r[0] + "\": " ;
o += r[1];
o += 1;
d += int32(o) + " ";
o += 4;
r = ascii(o, n);
d += ",\"" + r[0] + "\": " ;
o += r[1];
let nb_field = int32(o);
d += nb_field + " ";
o += 4;

let i = 0;
n = h.length - o;
while (i < n) {
	if (nb_field < 1) {
		break;
	}
	nb_field--;
	r = utf16(o + i, n - i);
	if (r[1] < 1) {
		break;
	}
	d += ",\n \"" + r[0] +"\":";
	i += r[1];
	i++;
	r = ascii(o + i, n - i);
	if (r[1] < 1) {
		break;
	}
	i += r[1];
	switch (r[0]) {
	case "RemoteableDouble":
	case "RemoteableBool":
	case "UserFloat":
	case "RemoteableEnum":
	case "RemoteableTimeSignature":
	case "RemoteableInt":
	case "RemoteableList":
	case "OnSets":
	case "OnsetArray":
	case "AufTaktData":
	case "SampleOverView":
		break;
	default:
		console.log("case \"" + r[0] + "\":");
	}
	d += " \"" + r[0] +"\" ";
}
o += i;
console.log("end : " + o + " = " + h.length);

d += "\n";
n = h.length - o;
i = 0;
let k;

i++;
r = ascii(o + i, n - i);
i += r[1];
d += " \"" + r[0] +"\": ";
let l = int32(o+i);
i += 4;
d += " " + l;
console.log(r[0] + " " + l);

i++;
r = ascii(o + i, n - i);
i += r[1];
l = int32(o+i);
i += 4;
d += "\n\"" + r[0] +"\": " + l ;
console.log(r[0] + " " + l);

r = utf16(o + i, n - i);
i += r[1];
l = h[o+i] + (h[o+i+1] << 8);
i += 2;
d += "\n\"" + r[0] +"\": " + l ;
let ex = 0;
k = 190;
let onsetevent = 0;
while (i < n) {
	k--;
	if (k == 0) {
		break;
	}
	r = ascii(o + i, n - i);
	i += r[1];
	l = int32(o+i);
	i += 4;
	d += "\n\"" + r[0] +"\": " + l ;
	console.log(r[0] + " MIC " + l);
	if (r[1] == 1) {
	console.log(r[0] + " SIIIII " + l);
		i += 18;
		continue;
	}
	if (ex) {
			process.exit(2);
	}
	switch (r[0]) {
	case "OnsetEvent":
		onsetevent++;
	console.log("onsetvent " + onsetevent);
		if (onsetevent == 2) {
			i -= 3;
			l = int32(o+i);
			i += 4;
			i += l;
			i += 25;
			console.log("KKK " + l + " == " + (o+i));
			//ex = 1;
		} else if (onsetevent == 1) {
			r = OnsetEvent(o+i, n-i,l);
			i += r[1];
			d += r[0];
		}
		l = 0;
		i++;
		break;
	case "SampleOverView":
		r = SampleOverView(o+i, n-i,l);
		i += r[1];
		d += r[0];
		l = 0;
		i++;
		break;
	case "SampleOverViewLevel":
		r = SampleOverViewLevel(o+i, n-i,l);
		i += r[1];
		d += r[0];
		l = 0;
		i++;
		break;
	case "AufTaktData":
		r = AufTaktData(o+i, n-i,l);
		i += r[1];
		d += r[0];
		l = 0;
		i++;
		break;




	}

if (l == 1) {
	r = utf16(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	l = h[o+i] + (h[o+i+1] << 8);
	i += 2;
} else if (l == 2) {
	r = utf16(o + i, n - i);
	i += r[1];
	l = h[o+i];
	d += "\n\"" + r[0] +"\": " + l ;
	i += 1;
	r = ascii(o + i, n - i);
	i += r[1];
	l = h[o+i];
	d += "\n\"" + r[0] +"\": " + l ;
	r = utf16(o + i, n - i);
	i += r[1];
	l = h[o+i];
	d += "\n\"" + r[0] +"\": " + l ;
	i += 1;
	r = ascii(o + i, n - i);
	i += r[1];
	l = h[o+i];
	d += "\n\"" + r[0] +"\": " + l ;
	i += 1;

} else if (l == 3) {
	r = utf16(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	l = h[o+i];
	i += 1;
	r = ascii(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	r = utf16(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	i += 1;
	r = ascii(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	r = utf16(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	i++;
	r = ascii(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	i++;
} else if (l == 4) {
	r = utf16(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	l = h[o+i];
	i += 1;
	r = utf16(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	l = h[o+i];
	i += 1;
	r = utf16(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	l = h[o+i];
	i += 1;
	r = utf16(o + i, n - i);
	i += r[1];
	d += "\n\"" + r[0] +"\": " + l ;
	l = h[o+i];
	i++;
	i++;
} else if (l == -3) {
	i++;
} else if (l == -1) {
	i++;
}
}

console.log(k + " K " + i);

/*
	while (l > 0) {
		l--;
		r = utf16(o + i, n - i);
		d += " \"" + r[0] +"\": ";
		i += r[1];
		d += " " + h[o+i];
		i++;
		i++;

		r = ascii(o + i, n - i);
		i += r[1];
		d += " \"" + r[0] +"\": ";
		let s = int32(o+i);
		i += 4;
		while (s > 0) {
			s--;
			d += " " + int32(o+i);
			i += 4;
		}
		
		
	}*/

d += "\n}\n";
fs.writeFileSync(process.argv[3], d, "utf8");
fs.appendFileSync(process.argv[3], "", "utf8");

