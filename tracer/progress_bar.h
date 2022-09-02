#ifndef PROGRESS_BAR_H_INCLUDED
#define PROGRESS_BAR_H_INCLUDED

#include "concurrent_trace.h"
#include <cinttypes>

namespace pbar
{

#include <ncurses/ncurses.h>

void init_curses()
{
    initscr();
    noecho();
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
}

bool print_progressbar(const std::vector <concurrent::block_info> & info)
{
    constexpr int name_witdh = 15;
    constexpr int bar_width = 50;
    bool is_completed = true;
    clear();
    for(auto itr = info.cbegin(); itr != info.cend(); ++itr)
    {
        int i = std::distance(info.cbegin(), itr);
        bool not_completed = itr->progress < itr->scanline_max - itr->scanline_min;

        if(!not_completed)
            attron(COLOR_PAIR(2));
        else
            attron(COLOR_PAIR(0));

        mvprintw(i, 0, "THREAD %02d" , i+1);

        mvaddch(i, name_witdh, '[');
        mvhline(i, name_witdh + 1, '=', itr->progress * bar_width / (itr->scanline_max - itr->scanline_min));
        mvaddch(i, name_witdh + bar_width + 1, ']');

        mvprintw(i, name_witdh + bar_width + 5, "%2d of %2d", itr->progress.load(), itr->scanline_max - itr->scanline_min);

        if(not_completed)
        {
            is_completed = false;
            attroff(COLOR_PAIR(0));
        }
        else
            attroff(COLOR_PAIR(2));

    }
    refresh();
    return is_completed;
}

void decon_curses()
{
    endwin();
}

};

#endif // PROGRESS_BAR_H_INCLUDED
