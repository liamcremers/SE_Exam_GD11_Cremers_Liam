#include "BreakOut.h"

const Color Color::White = Color(1.f, 1.f, 1.f);
const Color Color::Black = Color(0.f, 0.f, 0.f);
const Color Color::Red = Color(1.f, 0.f, 0.f);
const Color Color::Blue = Color(0.f, 0.f, 1.f);
const Color Color::Green = Color(0.f, 1.f, 0.f);

ToyPainter::ToyPainter(const SetupContext& setupCtx)
{}

ToyPainter::~ToyPainter()
{}

void ToyPainter::PollEvents()
{}

void ToyPainter::Present() const
{}

bool ToyPainter::IsRunning() const
{
	return false;
}

void ToyPainter::resizeWindow(int width, int height) const
{}

void ToyPainter::setTitle(const std::string & title) const
{}

void ToyPainter::Clear(const Color & color) const
{}

void ToyPainter::FillCircle(int x, int y, int radius, const Color & color) const
{}

void ToyPainter::FillCircle(int x, int y, int radius, const Color & color, int segments) const
{}

void ToyPainter::FillRect(int x, int y, int width, int height, const Color & color) const
{}
