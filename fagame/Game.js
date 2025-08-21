
// The authors disclaim copyright to this software.


class Game {
constructor(os) {
	this.os = os;
	this.os.usrDraw = this.draw.bind(this);
	this.os.usrClick = this.click.bind(this);
	this.os.usrMove = this.move.bind(this);
	this.os.usrKbdHit = this.kbdHit.bind(this);
	
	this.objs = new Array();
	this.objs.push(new Piece(200, 128, 64, 128));
	this.objs.push(new Piece(264, 140, 64, 108));
	this.o0 = new Man(50, 200, 32, 32);
	this.ground = 220;
	this.w = os.canvas.width;
	this.h = os.canvas.height;
}	


draw(c) {
	let o0 = this.o0;
	for (var o in this.objs) {
		this.objs[o].step(this);
	}
	this.o0.step(this);
	for (var o in this.objs) {
		//if (this.objs[o].hit(o0)) {
		//	this.objs[o].setState(Piece.STATE_HIT);
			this.o0.collision(this, this.objs[o]);
			//this.remove(this.objs[o]);
		//}
	}
	c.clearRect(0, 0, c.canvas.width, c.canvas.height);
	for (var o in this.objs) {
		this.objs[o].draw(c);
	}
	this.o0.draw(c);
}

kbdHit(e) {
	if (e.code == "ArrowLeft") {
		this.o0.addSpeed(-5, 0);
	}
	if (e.code == "ArrowRight") {
		this.o0.addSpeed(5, 0);
	}
	if (e.code == "ArrowUp") {
		this.o0.addSpeed(0, -10);
	}
	if (e.key == " ") {
		this.o0.addSpeed(0, -10);
		this.os.fire_noise();
	}
}

click(e) {
	this.o0.addSpeed(0, -10);
	this.os.fire_noise();
}

move(x, y) {
	//this.o0.move(x, y);
}

remove(o) {
	for (var i = 0; i < this.objs.length; i++) {
		if (this.objs[i] == o) {
			this.objs.splice(i, 1);
			return;
		}
	}	
}

} 