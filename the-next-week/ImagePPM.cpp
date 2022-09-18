#include "ImagePPM.h"


ImagePPM::ImagePPM(const std::filesystem::path& path, u32 width, u32 height)
{
	m_ofstream.open(path.string());
	write_header(width, height);
}

void ImagePPM::write_color(f32 r, f32 g, f32 b)
{
	u32 ir = (u8)(255.999 * r);
	u32 ig = (u8)(255.999 * g);
	u32 ib = (u8)(255.999 * b);
	m_ofstream << ir << ' ' << ig << ' ' << ib << '\n';
}

void ImagePPM::write_header(u32 width, u32 height)
{
	// PPM header
	m_ofstream << "P3\n" << width << ' ' << height << "\n255\n";
}
