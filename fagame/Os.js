
// The authors disclaim copyright to this software.

class Os {
constructor () {
	this.canvas = document.getElementById("canvas");
	this.ctx = this.canvas.getContext("2d");
	this.audio = new AudioContext();
	this.gain = this.audio.createGain();
	this.timer = setInterval(this.timerCb.bind(this), 50);

	this.gain.gain.setValueAtTime(0.005, this.audio.currentTime);
	this.gain.connect(this.audio.destination);
	this.osc = this.audio.createOscillator();
	this.osc.connect(this.gain);
	this.osc.ended = true;
	this.osc.onended = this.endedCb;

	window.addEventListener("mousemove", this.mousemoveCb.bind(this));
	window.addEventListener("click", this.clickCb.bind(this));
	window.addEventListener("keydown", this.keydownCb.bind(this));
	window.addEventListener("touchcancel", this.touchcancelCb.bind(this));
	window.addEventListener("touchend", this.touchendCb.bind(this));
	window.addEventListener("touchmove", this.touchmoveCb.bind(this));
	window.addEventListener("touchstart", this.touchstartCb.bind(this));
	this.touch = false;

	this.usrDraw = function (){};
	this.usrClick = function(){};
	this.usrMove = function(){};
	this.usrKbdHit = function(){};
}
 
endedCb(e) {
	e.target.ended = true;
}

timerCb() {
	this.usrDraw(this.ctx);
}

mousemoveCb(e) {
	var r = this.canvas.getBoundingClientRect();
	var x = e.clientX - r.left;
	var y = e.clientY - r.top;
	this.usrMove(x, y);
}

clickCb(e) {
	this.usrClick(e);
}

keydownCb(e) {
	this.usrKbdHit(e);
}

touchstartCb(e) {
	let t = e.touches[0];
	this.touch = {ox: t.clientX, oy: t.clientY, ex: 0, ey: 0};
}

touchendCb(e) {
	if (!this.touch) {
		return;
	}
	let dx = this.touch.ex - this.touch.ox;
	let dy = this.touch.ey - this.touch.oy;
	let dx2 = dx * dx;
	let dy2 = dy * dy;

	if (dx2 + dy2 < 2000) {
		let ev = {};
		var r = this.canvas.getBoundingClientRect();
		this.usrMove(this.touch.ex - r.left, this.touch.ey - r.top);
		this.usrClick(ev);
		return;
	}
	let ev =  {key: "", code:""};
	if (dx2 > dy2) {
		if (dx > 0) {
			ev.code = "ArrowRight";		
		} else {
			ev.code = "ArrowLeft";
		}
		this.usrKbdHit(ev);
		if (dx2 < dy2 * 4) {
			if (dy > 0) {
				ev.code = "ArrowDown";		
			} else {
				ev.code = "ArrowUp";
			}	
			this.usrKbdHit(ev);
		}
	} else {
		if (dy > 0) {
			ev.code = "ArrowDown";		
		} else {
			ev.code = "ArrowUp";
		}
		this.usrKbdHit(ev);
		if (dy2 < dx2 * 4) {
			if (dx > 0) {
				ev.code = "ArrowRight";		
			} else {
				ev.code = "ArrowLeft";
			}	
			this.usrKbdHit(ev);
		}
	}
}

touchmoveCb(e) {
	let t = e.touches[0];
	this.touch.ex = t.clientX;
	this.touch.ey = t.clientY;

}

touchcancelCb(e) {
	this.touch = false;
}

fire_noise() {
	//gain.gain.exponentialRampToValueAtTime(0.00001, audio.currentTime + 0.05);
	if (this.osc.ended) {
		this.osc.disconnect();
		this.osc = this.audio.createOscillator();
		this.osc.connect(this.gain);
		this.osc.type = "square";
		this.osc.frequency.setValueAtTime(300, this.audio.currentTime);
		this.osc.frequency.linearRampToValueAtTime(100, this.audio.currentTime + 0.2);
		this.osc.start(0);
		this.osc.stop(this.audio.currentTime + 0.2);
		this.osc.onended = this.endedCb;
		this.osc.ended = false;	
	}
}

} 