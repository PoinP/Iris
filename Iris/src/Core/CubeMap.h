#ifndef CUBE_MAP_H

#define CUBE_MAP_H

#include <vector>
#include <Core/Ray.h>
#include <Objects/Textures/ImageTexture.h>

class CubeMap
{
public:
	CubeMap(const Color& backgroundColor = Color());

	// Cube maps are passed in the following order: posx, posy, posz, negx, negy, negz
	CubeMap(const std::vector<std::string>& cubeMapPaths);

	Color getColor(const Ray& ray) const;

private:
	Color m_BackgroundColor;
	std::vector<ImageTexture> m_Textures;

	Vector2f getUVCoordinates(const Vector3f& direction, int& textureIndex) const;
};

#endif // !CUBEMAP_H
