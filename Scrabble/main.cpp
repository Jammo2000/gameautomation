
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include "boarddisplay.h"
#include "solver.h"
#include "board.h"
int main(int argc, char **argv)
{
	Fl_Window window(340, 340);
	BoardDisplay display(0,0,window.w(), window.h(), "Display It");
	display.setBoard(&board);
	board.applyMove(Move(7,6,true, "GOAT"));
	window.resizable(display);
	window.end();
	window.show(argc, argv);
	return Fl::run();
}