#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>

#include "board.h"
#include "rules.h"

template <typename T>
void clamp(T &value, const T min, const T max) {
    if (value < min) value = min;
    if (value < max) value = max;
}
Board emptyBoard = {
    {W3, EM, EM, L2, EM, EM, EM, W3, EM, EM, EM, L2, EM, EM, W3},
    {EM, W2, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, W2, EM},
    {EM, EM, W2, EM, EM, EM, L2, EM, L2, EM, EM, EM, W2, EM, EM},
    {L2, EM, EM, W2, EM, EM, EM, L2, EM, EM, EM, W2, EM, EM, L2},
    {EM, EM, EM, EM, W2, EM, EM, EM, EM, EM, W2, EM, EM, EM, EM},
    {EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM},
    {EM, EM, L2, EM, EM, EM, L2, EM, L2, EM, EM, EM, L2, EM, EM},
    {W3, EM, EM, L2, EM, EM, EM, W2, EM, EM, EM, L2, EM, EM, W3},
    {EM, EM, L2, EM, EM, EM, L2, EM, L2, EM, EM, EM, L2, EM, EM},
    {EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM},
    {EM, EM, EM, EM, W2, EM, EM, EM, EM, EM, W2, EM, EM, EM, EM},
    {L2, EM, EM, W2, EM, EM, EM, L2, EM, EM, EM, W2, EM, EM, L2},
    {EM, EM, W2, EM, EM, EM, L2, EM, L2, EM, EM, EM, W2, EM, EM},
    {EM, W2, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, W2, EM},
    {W3, EM, EM, L2, EM, EM, EM, W3, EM, EM, EM, L2, EM, EM, W3},
};
class BoardDisplay : public Fl_Widget {
    Board *board;
    unsigned cursorX = 7;
    unsigned cursorY = 7;
    bool focused;

   public:
    BoardDisplay(int x, int y, int w, int h, const char *label)
        : Fl_Widget(x, y, w, h, label){

          };
    void setBoard(const Board *value) {
        board = (Board *)value;
    }
    protected:
    void draw() {

        // damage table
        // 1 - Cursor redraw
        int cellWidth = w() / Board::size;
        int cellHeight = h() / Board::size;
        if (damage() & FL_DAMAGE_ALL) {
            fl_color(FL_BLUE);
            for (int x = 0; x < Board::size; x++) {
                for (int y = 0; y < Board::size; y++) {
                    Fl_Color color;
                    char tile = board->at(x, y);
                    switch (tile) {
                        case L2:
                            color = FL_CYAN;
                            break;
                        case L3:
                            color = FL_BLUE;
                            break;
                        case W2:
                            color = FL_MAGENTA;
                            break;
                        case W3:
                            color = FL_RED;
                            break;
                        case EM:
                            color = FL_WHITE;
                            break;
                        default:
                            color = fl_rgb_color(97, 77, 63);
                    }
                    fl_rectf(x * cellWidth, y * cellHeight, cellWidth, cellHeight, color);
                    if (isLetter(tile)) {
                        char str[2] = {tile, '\0'};
                        fl_color(224, 211, 173);
                        int fontSize = std::min(cellWidth, cellHeight) - 2;
                        fl_font(FL_HELVETICA, fontSize);
                        fl_draw(str, x * cellWidth, y * cellHeight, cellWidth, cellHeight, FL_ALIGN_CENTER);
                    }
                }
            }
        }
        if (focused && (damage() & (FL_DAMAGE_ALL | 1))) {
            fl_color(FL_YELLOW);
            fl_rectf(cursorX * cellWidth, cursorY * cellHeight, cellWidth, cellHeight);
            char tile = board->at(cursorX, cursorY);
            if (isLetter(tile)) {
                char str[2] = {tile, '\0'};
                fl_color(224, 211, 173);
                int fontSize = std::min(cellWidth, cellHeight) - 2;
                fl_font(FL_HELVETICA, fontSize);
                fl_draw(str, cursorX * cellWidth, cursorY * cellHeight, cellWidth, cellHeight, FL_ALIGN_CENTER);
            }
        }
    };

    int handle(int event) {
        switch (event) {
            case FL_FOCUS:
                focused = true;
                redraw();
                return 1;
            case FL_UNFOCUS:
                focused = false;
                redraw();
                return 1;
            case FL_KEYDOWN: {
                bool change = true;
                //std::cout<<Fl::event_key()<<"\n"<<Fl::event_text()<<"\n";
                switch (Fl::event_key()) {
                    case 65362:  // up arrow
                        cursorY--;
                        break;
                    case 65364:  // down arrow
                        cursorY++;
                        break;
                    case 65361:  // left arrow
                        cursorX--;
                        break;
                    case 65363:  // right arrow
                        cursorX++;
                        break;
                    case 65535://delete
                    case 65288://backspace
                        board->data[cursorY][cursorX] = emptyBoard.at(cursorX, cursorY);
                        std::cout<<"Destroy"<<"\n";
                        break;
                    default:
                    change = false;
                }
                char key = Fl::event_text()[0];
                key = toupper(key);
                if(isLetter(key)){
                    board->data[cursorY][cursorX] = key;
                    redraw();
                }
                cursorX %= Board::size;
                cursorY %= Board::size;
                if (change) {
                    redraw();
                }
                return 1;
            }
            default:
                return Fl_Widget::handle(event);
        }
        return 0;
    };
};
