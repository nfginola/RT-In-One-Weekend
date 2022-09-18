#pragma once
#include <filesystem>
#include <fstream>
#include "Common.h"

class ImagePPM
{
public:
	ImagePPM(const std::filesystem::path& path, u32 width, u32 height);

	// [0.0, 1.0]
	void write_color(f32 r, f32 g, f32 b);


private:
	void write_header(u32 width, u32 height);
	
private:
	std::ofstream m_ofstream;

};
