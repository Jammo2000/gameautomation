
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Window.H>
#include <stdarg.h>

#include <algorithm>
#include <cstring>

#include "alphabetset.h"
#include "board.h"
#include "boarddisplay.h"
#include "solver.h"

struct SolverParams {
    Fl_Input* tileInput;
    BoardDisplay* boardDisplay;
    Fl_Multiline_Output* statusOutput;
};

void solve(Fl_Widget* button, void* param) {
    SolverParams* widgets = reinterpret_cast<SolverParams*>(param);
    std::string tileString = std::string(widgets->tileInput->value());
    char msgBuf[30000];
    msgBuf[0] = '\0';
    // why tf does fltk use c strings? Its a C++ library
    auto printMsg = [&widgets, &msgBuf](const char* msg, ...) {
        unsigned spaceLeft = sizeof(msgBuf) - strlen(msgBuf) - 1;  // null terminator isnt in strlen;
        strncat(msgBuf, "\n", spaceLeft);
        spaceLeft = sizeof(msgBuf) - strlen(msgBuf);
        va_list args;
        va_start(args, msg);
        printf("Buffer: %s\nMessage: ", msgBuf);
        printf("\n");
        vsnprintf(msgBuf + strlen(msgBuf), spaceLeft, msg, args);
        va_end(args);
        widgets->statusOutput->value(msgBuf);
        widgets->statusOutput->redraw();
    };
    // Have to make tiles uppercase
    std::transform(tileString.begin(), tileString.end(), tileString.begin(), ::toupper);
    AlphabetSet tileSet(tileString);
    printMsg("Finding Moves...");
    std::vector<Move> legalMoves = getLegalMoves(board, tileSet);
    if (legalMoves.size() == 0) {
        printMsg("No legal moves found");
        return;
    }
    printMsg("Found some legal moves", legalMoves.size());
    unsigned bestValue = 0;
    Move* bestMove = NULL;
    printMsg("Calculating point values...");
    for (Move& move : legalMoves) {
        unsigned value = move.getPoints(board);
        if (value > bestValue) {
            bestMove = &move;
            bestValue = value;
        }
    }
    printMsg("Found \"%s\" for %u points", bestMove->word.c_str(), bestValue);
    board.applyMove(*bestMove);
    widgets->boardDisplay->redraw();
}
int main(int argc, char** argv) {
    Fl_Window win(900, 450);
    BoardDisplay display(0, 0, win.h(), win.h(), "Display It");
    display.setBoard(&board);

    Fl_Group controlGroup(win.h(), 0, win.w() - win.h(), win.h());
    controlGroup.box(FL_UP_BOX);
    int x = win.h() + (controlGroup.w() - 100) / 2;
    Fl_Input tileInput(x, 80, 100, 30, "Enter Letters Here");
    tileInput.align(FL_ALIGN_TOP);

    Fl_Button solveButton(x, 400, 100, 30, "Solve");
    x = win.h() + (controlGroup.w() - 200) / 2;
    Fl_Multiline_Output statusDisplay(x, 120, 200, 200);

    statusDisplay.value("Ready...");
    controlGroup.end();
    SolverParams params{&tileInput, &display, &statusDisplay};
    solveButton.callback(solve, &params);
    win.end();
    win.show(argc, argv);
    return Fl::run();
}