#include "widerangespinbox.h"


WideRangeSpinBox::WideRangeSpinBox(QWidget *parent) : QDoubleSpinBox(parent)
{
	setRange(-FLT_MAX, FLT_MAX);
}


WideRangeSpinBox::~WideRangeSpinBox()
{
}
