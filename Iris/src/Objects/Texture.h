#ifndef TEXTURE_H

#define TEXTURE_H

#include <Containers/Color.h>

class Texture
{
public:
	virtual ~Texture() {};
	virtual Color getColor(float u, float v) const = 0;

private:
};

#endif // !TEXTURE_H
