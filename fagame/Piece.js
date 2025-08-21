
// The authors disclaim copyright to this software.

class Piece {

static STATE_VISIBLE = 1;
static STATE_HIT = 2;
static STATE_REMOVE = 4;

constructor (x, y, w, h) {
	this.x = x; // left
	this.y = y; // top
	this.w = w;
	this.h = h; 
	this.state = Piece.STATE_VISIBLE;
	this.xspeed = -1;
	this.yspeed = 0;
	this.g = 9.81;
}

setState(s) {
	this.state = s;
}

addSpeed (x, y) {
	this.xspeed += x;
	this.yspeed += y;
}

draw(c) {
	c.beginPath();
	c.moveTo(this.x, this.y);
	c.lineTo(this.x + this.w, this.y);
	c.lineTo(this.x + this.w, this.y + this.h);
	c.lineTo(this.x, this.y + this.h);
	c.lineTo(this.x, this.y);
	c.stroke();
	c.closePath();	
}

move(x, y) {
	this.x = x;
	this.y = y;
}

step(world) {
	this.x += this.xspeed;
	this.y += this.yspeed;
	this.yspeed += this.g / 20;

	switch (this.state) {
	case Piece.STATE_VISIBLE:
		break;
	case Piece.STATE_HIT:
		this.x += -1;
		break;
	
	}
	if (this.y + this.h > world.ground) {
		this.y = world.ground - this.h;
		this.yspeed = 0;
	}
	if (this.x + this.w < 0) {
		this.x = world.w;
		this.h = 40 + 60 * Math.random();
	}
}

hit(o) {
	if (o === this) {
		return false;
	}
	if (o.x + o.w >= this.x && o.x < this.x + this.w) {
		if (o.y + o.h >= this.y && o.y < this.y + this.h) {
			return true;
		}
	}
	return false;
}

} 