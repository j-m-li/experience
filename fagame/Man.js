
// The authors disclaim copyright to this software.

class Man extends Piece {
constructor(x, y, w, h) {
	super(x, y, w, h);
	this.xspeed = 1.0;
	this.floor = 0;
}

draw(c) {
	c.beginPath();
	c.moveTo(this.x, this.y);
	c.quadraticCurveTo(this.x + this.w, this.y, this.x + this.w, this.y + this.h);
	c.lineTo(this.x, this.y + this.h);
	c.lineTo(this.x, this.y);
	c.stroke();
	c.closePath();	
}

step(world) {
	this.x += this.xspeed;
	this.y += this.yspeed;
	this.yspeed += this.g / 20;

	if (this.y + this.h > world.ground) {
		this.y = world.ground - this.h;
		this.yspeed = 0;
	}

	if (this.x >= world.w / 2 + this.w) {
		this.x = world.w / 2 + this.w;
		this.xspeed = 0;
	} else if (this.x <= world.w / 2 - 2 * this.w){
		this.x = world.w / 2 - this.w * 2;
		this.xspeed = 0;
	}
	this.floor = world.ground;
}

collision(world, o) {
	if (o.x + o.w >= this.x && o.x < this.x + this.w) {
		if (o.y <= this.floor) {
			this.floor = o.y;
			if (this.y + this.h > this.floor) {
				this.y = this.floor - this.h;
				this.yspeed = 0;
			}
		}
	}
}

}
