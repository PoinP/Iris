#include "CubeMap.h"

#include <assert.h>
#include <Objects/Materials/Emissive.h>

CubeMap::CubeMap(const Color& backgroundColor)
    : m_BackgroundColor(backgroundColor)
{
}

CubeMap::CubeMap(const std::vector<std::string>& cubeMapPaths)
{
    assert(cubeMapPaths.size() == 6);
    m_Textures.reserve(6);

    for (const std::string& cubeMapPath : cubeMapPaths)
        m_Textures.emplace_back(cubeMapPath);
}

Color CubeMap::getColor(const Ray& ray) const
{
    if (m_Textures.empty())
        return m_BackgroundColor;

    int textureIndex;
    Vector2f uv = getUVCoordinates(ray.direction(), textureIndex);
    return m_Textures[textureIndex].getColor(uv.x(), uv.y());
}

Vector2f CubeMap::getUVCoordinates(const Vector3f& direction, int& textureIndex) const
{
    // Indexes  | x | y | z |
    // Positive | 0 | 1 | 2 |
    // Negative | 3 | 4 | 5 |

    int maxAxisIndex = 0;
    float maxAxis = fabs(direction[maxAxisIndex]);

    // Find maximum abolute axis and its index
    for (int i = 1; i < 3; i++)
    {
        float absDir = fabs(direction[i]);
        if (maxAxis < absDir)
        {
            maxAxis = absDir;
            maxAxisIndex = i;
        }
    }

    // Normalize the direction vector by the absolute direction
    // This way we the max axis value is either -1 or 1.
    // This alows us to get the texture index easily.
    Vector3f normAxis = direction / fabs(direction[maxAxisIndex]);

    // If max axis is positive then texture index is between [0, 2] else [3, 5]
    textureIndex = (normAxis[maxAxisIndex] > 0.0f ? 0 : 3) + maxAxisIndex;

    const float x = direction.x();
    const float y = -direction.y();
    const float z = direction.z();

    float uc, vc;
    switch (textureIndex)
    {
        case 0: uc = -z; vc =  y; break;
        case 1: uc =  x; vc = -z; break;
        case 2: uc =  x; vc =  y; break;
        case 3: uc =  z; vc =  y; break;
        case 4: uc =  x; vc = -z; break;
        case 5: uc = -x; vc =  y; break;
    }

    // Convert range from -1 to 1 to 0 to 1
    float u = 0.5f * (uc / maxAxis + 1.0f);
    float v = 0.5f * (vc / maxAxis + 1.0f);

    return Vector2f(u, v);
}
