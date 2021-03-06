#pragma once
#ifndef DS_ARC_ARCCOLORARRAY_H_
#define DS_ARC_ARCCOLORARRAY_H_

#include "ds/arc/arc_input.h"

namespace ds {
namespace arc {

/**
 * \class ds::arc::ColorArray
 * \brief A standard color object -- store 1 or more colors, and let clients
 * access them with a unit coordinate. With 2 or more colours, the coord interpolates;
 * if it's 1 color, then it modulates the alpha.
 */
class ColorArray
{
public:
	ColorArray();

	ci::ColorA				at(const Input&, const double unit) const;

	virtual void			readXml(const ci::XmlTree&);

private:
	std::vector<ColorParam>	mColor;
};

} // namespace arc
} // namespace ds

#endif // DS_ARC_ARCCOLORARRAY_H_