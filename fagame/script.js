
// The authors disclaim copyright to this software.

include("./Os");
include("./Game");
include("./Piece");
include("./Man");

TheApp = class TheApp {
	static main()
	{
		TheApp.os = new Os();
		TheApp.game = new Game(this.os);
	}
}