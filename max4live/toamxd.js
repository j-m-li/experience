const fs = require('fs');

console.log("hello");

data = fs.readFileSync(process.argv[3]);

const h = new Uint8Array(32);

h[0] = 0x61; 
h[1] = 0x6d; 
h[2] = 0x70; 
h[3] = 0x66; 
h[4] = 0x04; 
h[5] = 0x0; 
h[6] = 0x0; 
h[7] = 0x0; 
h[8] = 0x6d; 
h[9] = 0x6d; 
h[10] = 0x6d; 
h[11] = 0x6d; 
h[12] = 0x6d; 
h[13] = 0x65; 
h[14] = 0x74; 
h[15] = 0x61; 

h[16] = 0x04; 
h[17] = 0x0; 
h[18] = 0x0; 
h[19] = 0x0; 
h[20] = 0x0; 
h[21] = 0x0; 
h[22] = 0x0; 
h[23] = 0x0; 
h[24] = 0x70; 
h[25] = 0x74; 
h[26] = 0x63; 
h[27] = 0x68; 
h[28] = data.length & 0xFF; 
h[29] = (data.length >> 8) & 0xFF; 
h[30] = (data.length >> 16) & 0xFF; 
h[31] = (data.length >> 24) & 0xFF; 

switch (process.argv[2]) {
case "-i":
	h[8] = 0x69; 
	h[9] = 0x69; 
	h[10] = 0x69; 
	h[11] = 0x69; 
	break;
case "-a":
	h[8] = 0x61; 
	h[9] = 0x61; 
	h[10] = 0x61; 
	h[11] = 0x61; 
	break;
case "-m":
	break;
}
fs.writeFileSync(process.argv[4], h, "binary");
fs.appendFileSync(process.argv[4], data, "binary");
const a = new Uint8Array(1);
a[0] = 0;
fs.appendFileSync(process.argv[4], a, "binary");

