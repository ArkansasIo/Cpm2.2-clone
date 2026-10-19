#include "cpm/disk_image.hpp"
#include <fstream>
namespace cpm22::cpm {
DiskImage::DiskImage(DiskParameterBlock dpb,std::size_t tracks):dpb_(dpb),disk_(tracks*dpb_.sectors_per_track,128){}
bool DiskImage::read_sector(std::size_t track,std::size_t sector,std::vector<std::uint8_t>& out){if(sector>=dpb_.sectors_per_track)return false;return disk_.read(track*dpb_.sectors_per_track+sector,out);}
bool DiskImage::write_sector(std::size_t track,std::size_t sector,const std::vector<std::uint8_t>& data){if(sector>=dpb_.sectors_per_track)return false;return disk_.write(track*dpb_.sectors_per_track+sector,data);}
bool DiskImage::save(const std::string& path) const {std::ofstream f(path,std::ios::binary);if(!f)return false;std::vector<std::uint8_t> s;for(std::size_t i=0;i<disk_.sector_count();++i){const_cast<devices::VirtualDisk&>(disk_).read(i,s);f.write(reinterpret_cast<const char*>(s.data()),static_cast<std::streamsize>(s.size()));}return static_cast<bool>(f);}
bool DiskImage::load(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)return false;std::vector<std::uint8_t> s(128);for(std::size_t i=0;i<disk_.sector_count();++i){f.read(reinterpret_cast<char*>(s.data()),128);if(f.gcount()!=128)return false;if(!disk_.write(i,s))return false;}return true;}
}