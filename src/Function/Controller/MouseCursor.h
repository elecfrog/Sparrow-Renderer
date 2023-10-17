struct MousePosition
{
	double x{}, y{};
};

struct MouseCursor
{
	bool bIsDragging{ false };
	MousePosition start{};
	MousePosition curr{};
};
