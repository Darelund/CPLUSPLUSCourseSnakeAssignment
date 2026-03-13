#include "UI.h"
#include "Position.h"

void button::draw()
{
    drawString(text.pos.x, text.pos.y, text.text, text.normalColor);
}
void button::selecteddraw()
{
    drawString(text.pos.x, text.pos.y, text.text, FOREGROUND_RED);
}
void text::draw()
{
    drawString(pos.x, pos.y, text, normalColor);
}