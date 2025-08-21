//
//      The authors and contributors disclaim copyright, 
//      patents and all related rights to this software.
//

class stl3d {
constructor()
{
	this.buf = "";	
}

println(str)
{
	console.log(this.buf + str);
	this.buf = "";
}


print(str)
{
	this.buf += str;
}

printn(n)
{
	this.buf += n|0;
}

printf(n)
{
	n = n;
	if (n < 0) {
		this.print("-");
		n = -n;
	}
	this.printn(n / 1000);
	if ((n % 1000) > 0) {
		this.print(".");
		this.printn(n % 1000 / 100);
		this.printn(n % 100 / 10);
		this.printn(n % 10);
	}
}

inv_sqrt(x)
{
	if (x <= 0) {
		return 0;
	}
	var y = 100;
	var last = 0;
	for (var i = 0; i < 15; i++) {
		var y2 = ((y * y)) / 1000;
		var xy2 = ((x * y2)) / 1000;
		var factor = 1500 - (((500 * xy2)) / 1000);
		y = ((y * factor))  / 1000;
		if (y == last) {
			break;
		}
		last = y;
	}
	return y;
}

mul(x, y)
{
	var v  = x * y;
	if (v < 0) {
		return ((v - 500) / 1000) | 0;
	}
	return ((v + 500) / 1000) | 0;
}



solid_begin(name)
{
	this.print("solid ASCII_STL_");
	this.println(name);
	return new Array(name,0,0);
}

solid_end(obj)
{
	this.print("endsolid ASCII_STL_");
	this.println(obj[0]); // name
}

vertex(x,y,z)
{
	return new Array(x,y,z);
}

vertex_x(a)
{
	return a[0];
}

vertex_y(a)
{
	return a[1];
}

vertex_z(a)
{
	return a[2];
}

add(a,x,y,z)
{
	return this.vertex(this.vertex_x(a) + x, 
		this.vertex_y(a) + y, this.vertex_z(a) + z);
}

print_vertex(a)
{
	this.print("vertex ");
	this.printf(a[0]);
	this.print(" ");
	this.printf(a[1]);
	this.print(" ");
	this.printf(a[2]);
	this.println("");
}

facet(a,b,c)
{
	var v1x = b[0] - a[0];
	var v1y = b[1] - a[1];
	var v1z = b[2] - a[2];
	var v2x = c[0] - a[0];
	var v2y = c[1] - a[1];
	var v2z = c[2] - a[2];
	// matrix cross product
	var crossx = ((v1y * v2z) - (v1z * v2y)) / 1000;
	var crossy = ((v1z * v2x) - (v1x * v2z)) / 1000;
	var crossz = ((v1x * v2y) - (v1y * v2x)) / 1000;
	var inv_magnitude = this.inv_sqrt((crossx*crossx + 
		crossy*crossy + crossz*crossz) / 1000);
	if (inv_magnitude == 0) {
		this.println("facet normal 0 0 0");
	} else {
		this.print("facet normal ");
		this.printf(this.mul(crossx,inv_magnitude));
		this.print(" ");
		this.printf(this.mul(crossy, inv_magnitude));
		this.print(" ");
		this.printf(this.mul(crossz, inv_magnitude));
		this.println("");
	}

	this.println("outer loop");
	this.print_vertex(a);
	this.print_vertex(b);
	this.print_vertex(c);
	this.println("endloop");
	this.println("endfacet");
}

box(pos, ww, hh, dd)
{
/*
the order of the triangle vertices using the "right-hand rule", i.e. the vertices are listed in counter-clock-wise order from outside.
*/
	var w2 = ww / 2;
	var h2 = hh / 2;
	var d2 = dd / 2;
	var a = this.add(pos,-w2,-h2,-d2);
	var b = this.add(pos,-w2,h2,-d2);
	var c = this.add(pos,w2,h2,-d2);
	var d = this.add(pos,w2,-h2,-d2);
	this.facet(a,b,c);
	this.facet(c,d,a);
	var e = this.add(pos,-w2,-h2,d2);
	var f = this.add(pos,-w2,h2,d2);
	var h = this.add(pos,w2,h2,d2);
	var i = this.add(pos,w2,-h2,d2);
	this.facet(h,f,e);
	this.facet(e,i,h);

}

cube(offset, size)
{
	this.box(offset, size, size, size);
}

} // stl3d

function setup()
{
	var s = new stl3d();
	var c = s.solid_begin("cube");
	var offset = s.vertex(0,0,0);
	s.cube(offset, 3000);
	s.solid_end(c);
}


function loop()
{
	process.exit(0);
}

setup();

loop();

